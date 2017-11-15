/*
 * shutter.c
 *
 * Created: 26-10-2017 16:02:35
 * Author: melle
 */ 

#define F_CPU 16E6

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>
#include "io/io.h"
#include "analog/analog.h"
#include "scheduler/scheduler.h"
#include "serial/serialconnection.h"
#include "hcsr04/hcsr04.h"
#include "storage/eeprom.h"
#include "helpers.h"
#include "shutter.h"

#define TMP_PIN 0 // TMP36 pin is on A0
#define LDR_PIN 1 // LDR pin is on A1

// LED's to give the status of the shutter
#define LEDRED			9
#define LEDGREEN		8
#define LEDYELLOW		10

// ID's are used for serial communication
#define TMP36			1
#define LDR				2
#define SHUTTER			3

// message types
#define MAX_UP_DIST		1
#define MAX_DOWN_DIST	2
#define ROLL			3
#define TMP_UPPER_LIMIT 4
#define TMP_LOWER_LIMIT 5
#define LDR_UPPER_LIMIT 6
#define LDR_LOWER_LIMIT 7

float temperatures[MAX_TMP_READINGS];
uint16_t lightvalues[MAX_LDR_READINGS];

int t = 0;
int l = 0;

enum state shutter_state = UP;
enum state next_state;
uint8_t PROCESS_ID;

settings shutter_settings;

void loadSettings() {
	shutter_settings.maxTmp = loadMaxTmp();
	shutter_settings.minTmp = loadMinTemp();
	shutter_settings.maxDownDist = loadMaxDownDist();
	shutter_settings.maxUpDist = loadMaxUpDist();
	shutter_settings.maxLight = loadMaxLight();
	shutter_settings.minLight = loadMinLight();
}

void initShutter() {
	outputPin(LEDRED);
	outputPin(LEDGREEN);
	outputPin(LEDYELLOW);
	
	setPin(LEDGREEN, HIGH);

	loadSettings();

	setSerialUpdateTrigger(controllerInputInterrupt);
	// initialize HCSR04
	initHCSR04();
}

void readTemperature() {
	uint16_t reading = readADC(TMP_PIN);

	float voltage = (reading * 5.0) / 1024.0;
	float temperature = (voltage - 0.5) * 100;
	temperatures[t] = temperature;

	t = (t >= MAX_TMP_READINGS-1) ? 0 : t + 1;
}

void readLightValue() {
	lightvalues[l] = map(readADC(LDR_PIN), 0, 1023, 0, 100);
	l = (l >= MAX_LDR_READINGS-1) ? 0 : l + 1;
}
 
float calculateAverageTemperature() {
	float total = 0.0;
	uint8_t validReadings = 0;

	for (int i = 0; i < MAX_TMP_READINGS; i++) {
		if(temperatures[i] != 0) validReadings++;
		total += temperatures[i];
	}

	return total / validReadings;
}
 
float calculateAverageLightIntensity() {
	float total = 0.0;
	uint8_t validReadings = 0;
	
	for (int i = 0; i < MAX_LDR_READINGS; i++) {
		if(lightvalues[i] != 0) validReadings++;
		total = total + lightvalues[i];
	}
	
	return total / validReadings;
}

void sendStatusUpdate() {
	char avgTempS[10];
	dtostrf(calculateAverageTemperature(), 2, 2, avgTempS);
	char avgLightS[10];
	dtostrf(calculateAverageLightIntensity(), 2, 2, avgLightS);
	printf("%u:%s|%u:%s|%u:%u\n",TMP36, avgTempS, LDR, avgLightS, SHUTTER, measureDistance());
}

// process a given command
void processMessage(uint8_t msg[2]) {
	uint8_t id = msg[0];
	uint8_t val = msg[1];
	switch(id) {
		case MAX_UP_DIST:
			saveMaxUpDist(val);
			shutter_settings.maxUpDist = val;
			break;
		case MAX_DOWN_DIST:
			saveMaxDownDist(val);
			shutter_settings.maxDownDist = val;
			break;
		case ROLL:
			val &= 1; // make sure 2th message bit is 0 or 1
			PROCESS_ID = SCHAddTask(roll, 0, 500);
			next_state = val;
			break;
		case TMP_UPPER_LIMIT:
			saveMaxTmp(val);
			shutter_settings.maxTmp = val;
			break;
		case TMP_LOWER_LIMIT:
			saveMinTemp(val);
			shutter_settings.minTmp = val;
			break;
		case LDR_UPPER_LIMIT:
			saveMaxLight(val);
			shutter_settings.maxLight = val;
			break;
		case LDR_LOWER_LIMIT:
			saveMinLight(val);
			shutter_settings.minLight = val;
			break;
	}
}

// this function runs when a byte is received from controller (python)
void controllerInputInterrupt(uint8_t byte) {
	static uint8_t bytesread = 0; // keep track of how many bytes we have read already. This is also used as index
	static uint8_t message[2]; // In here, we store the message
	// keep reading bytes, till we have 2
	message[bytesread] = byte;
	bytesread++;
	if(bytesread == 2) {
		processMessage(message);
		bytesread = 0;
	}
}

void toggleYellowLed() {
	static uint8_t flag = LOW;
	if(flag == LOW) {
		setPin(LEDYELLOW, HIGH);
		flag = HIGH;
	} else {
		setPin(LEDYELLOW, LOW);
		flag = LOW;
	}
}

void roll() {
	// do nothing if it's already in given state or given state is PROGRESS
	// TODO: make this if statement nicer!
	enum state s = next_state;
	if (!(s == shutter_state)) {
		uint16_t currentDist;
		if (s == UP) {
			// roll up
			setPin(LEDRED, LOW);
			shutter_state = PROGRESS_UP;
			// keep checking the distance, till shutter is up
			if(shutter_state != s) {
				toggleYellowLed();
				currentDist = measureDistance();
				// check if shutter is rolled up to max distance
				if(currentDist <= shutter_settings.maxUpDist) {
					shutter_state = UP;
					setPin(LEDGREEN, HIGH);
					setPin(LEDYELLOW, LOW);
					printf("%u:%u\n", SHUTTER, measureDistance());
					SCHDeleteTask(PROCESS_ID);
				}
			}
		} else if(s == DOWN) {
			// roll down
			setPin(LEDGREEN, LOW);
			shutter_state = PROGRESS_DOWN;
			if(shutter_state != s) {
				toggleYellowLed();
				currentDist = measureDistance();
				// check if shutter is rolled up to max distance
				if(currentDist >= shutter_settings.maxDownDist) {
					shutter_state = DOWN;
					setPin(LEDRED, HIGH);
					setPin(LEDYELLOW, LOW);
					printf("%u:%u\n", SHUTTER, measureDistance());
					SCHDeleteTask(PROCESS_ID);
				}
			}
		}
	};
}

// void emulateRoll() {
// 	// emulate doing something
// 	for (uint8_t i = 0; i < 5;i++)
// 	{
// 		setPin(LEDYELLOW, HIGH);
// 		_delay_ms(500);
// 		setPin(LEDYELLOW, LOW);
// 		_delay_ms(500);
// 	}
// }
