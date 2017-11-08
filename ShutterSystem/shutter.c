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
 
 // temperature & light readings
 float temperatures[MAX_TMP_READINGS];
 uint16_t lightvalues[MAX_LDR_READINGS];
 
 
 int t = 0;
 int l = 0;
 
 // set shutter state to up
 enum state shutter_state = UP;
 
 // set the leds to a pin
 void initShutter() {
	outputPin(LEDRED);
	outputPin(LEDGREEN);
	outputPin(LEDYELLOW);
	// green led is standard high, because shutter is up by default
	setPin(LEDGREEN, HIGH);
 }
 
 // reading & storing temperature
 void readTemperature() {
	uint16_t reading = readADC(TMP_PIN);
	// 5v divided by 1024
	float voltage = (reading * 5.0) / 1024.0;
	// setting temperature in Celsius in variable temperature
	float temperature = (voltage - 0.5) * 100;
	// adding temperature to array temperatures[t]
	temperatures[t] = temperature;

	char tmpS[10];
	dtostrf(temperature, 2, 2, tmpS);
	printf("tmp: %sC\n", tmpS);
	// if t is between 0 and 39, temperatures + 1
	// if t is > 39, start at index 0 and overwrite this value
	t = (t >= MAX_TMP_READINGS-1) ? 0 : t + 1;
 }
 
 // reading & storing lightvalue
 void readLightValue() {
	// mapping the lightvalue from 0-1023 to 0-100%
	// adding the lightvalue in array lightvalues[l]
	lightvalues[l] = map(readADC(LDR_PIN), 0, 1023, 0, 100);
	// if l is between 0 and 29, lightvalues + 1
	// if l is > 29, start at index 0 and overwrite this value
	l = (l >= MAX_LDR_READINGS-1) ? 0 : l + 1;
 }
 
 // calculating average temperature
 float calculateAverageTemperature() {
	 float total = 0.0;
	 uint8_t validReadings = 0;
	 
	 // looping through temperatures[t]
	 for (int i = 0; i < MAX_TMP_READINGS; i++) {
		// if the index is not 0, add 1 to validReadings
		if(temperatures[i] != 0) validReadings++;
		// total is the sum of all temperatures in temperatures[t]
		total += temperatures[i];
	 }
	 
	 // returning the average temperature, by dividing total with validReadings
	 return total / validReadings;
 }
 
 // calculating the average lightintensity
 float calculateAverageLightIntensity() {
	 float total = 0.0;
	 uint8_t validReadings = 0;
	 
	 // looping through lightvalues[l]
	 for (int i = 0; i < MAX_LDR_READINGS; i++) {
		// if the index is not 0, add 1 to validReadings
		if(lightvalues[i] != 0) validReadings++;
		// total is the sum of all lightvalues in lightvalues[l]
		total = total + lightvalues[i];
	 }
	 
	 // returning the average lightintensity, by dividing total with validReadings
	 return total / validReadings;
 }

 void sendStatusUpdate() {
	char avgTempS[10];
	dtostrf(calculateAverageTemperature(), 2, 2, avgTempS);
	char avgLightS[10];
	dtostrf(calculateAverageLightIntensity(), 2, 2, avgLightS);
	printf("%u:%s|%u:%s\n",TMP36, avgTempS, LDR, avgLightS);
 }

 // this function runs when a byte is received from controller (python)
 void controllerInputInterrupt(uint8_t byte) {
	if(byte == DOWN) {
		printf("rolling down\n");
		roll(DOWN);
	} else {
		printf("rolling up\n");
		roll(UP);
	}
	printf("DONE\n");
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
 
