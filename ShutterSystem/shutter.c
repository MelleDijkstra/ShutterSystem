/*
 * shutter.c
 *
 * Created: 26-10-2017 16:02:35
 * Author: melle
 */ 

 #include <stdbool.h>
 #include <stdint.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include "analog/analog.h"
 #include "serial/serialconnection.h"
 #include "helpers.h"
 #include "shutter.h"

 #define TMP_PIN 0 // TMP36 pin is on A0
 #define LDR_PIN 1 // LDR pin is on A1

 // ID's are used for serial communication
 #define TMP36		1
 #define LDR		2
 #define SHUTTER	3

 float temperatures[MAX_TMP_READINGS];
 uint16_t lightvalues[MAX_LDR_READINGS];

 int t = 0;
 int l = 0;
 bool shutter_state = false;

 void readTemperature() {
	uint16_t reading = readADC(TMP_PIN);

	float voltage = (reading * 5.0) / 1024.0;
	float temperature = (voltage - 0.5) * 100;
	temperatures[t] = temperature;

	char tmpS[10];
	dtostrf(temperature, 2, 2, tmpS);
	printf("tmp: %sC\n", tmpS);

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

 void controllerInputInterrupt(uint8_t byte) {
	transmit8(byte);
 }