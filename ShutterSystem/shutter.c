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
#include "serial/serialconnection.h"
#include "helpers.h"
#include "shutter.h"

#define TMP_PIN 0 // TMP36 pin is on A0
#define LDR_PIN 1 // LDR pin is on A1

// LED's to give the status of the shutter
#define LEDRED		8
#define LEDGREEN	9
#define LEDYELLOW	10

// ID's are used for serial communication
#define TMP36		1
#define LDR		2
#define SHUTTER	3

float temperatures[MAX_TMP_READINGS];
uint16_t lightvalues[MAX_LDR_READINGS];

int t = 0;
int l = 0;

enum state shutter_state = UP;

void initShutter() {
	outputPin(LEDRED);
	outputPin(LEDGREEN);
	outputPin(LEDYELLOW);
	
	setPin(LEDGREEN, HIGH);

	setSerialUpdateTrigger(controllerInputInterrupt);
	// TODO: read HCSR04 to get the state of the shutter
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
	printf("%u:%s|%u:%s\n",TMP36, avgTempS, LDR, avgLightS);
}

void processMessage(uint8_t msg[2]) {
	if(msg[0] == 3) {
		if(msg[1] == DOWN) {
			roll(DOWN);
		} else if (msg[1] == UP) {
			roll(UP);
		}	 
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

void roll(enum state s) {
	// do nothing if it's already in given state or given state is PROGRESS
	// TODO: make this if statement nicer!
	if (s == shutter_state || shutter_state == PROGRESS_UP || shutter_state == PROGRESS_DOWN) return;
	if (s == UP) {
		// roll up
		setPin(LEDRED, LOW);
		shutter_state = PROGRESS_UP;
		emulateRoll();
		shutter_state = UP;
		setPin(LEDGREEN, HIGH);
	} else if(s == DOWN) {
		// roll down
		setPin(LEDGREEN, LOW);
		shutter_state = PROGRESS_DOWN;
		emulateRoll();
		shutter_state = DOWN;
		setPin(LEDRED, HIGH);
	}
}

void emulateRoll() {
	// emulate doing something
	for (uint8_t i = 0; i < 5;i++)
	{
		setPin(LEDYELLOW, HIGH);
		_delay_ms(500);
		setPin(LEDYELLOW, LOW);
		_delay_ms(500);
	}
}
