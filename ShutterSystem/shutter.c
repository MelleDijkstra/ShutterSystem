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

	t = (t >= MAX_TMP_READINGS) ? 0 : t + 1;
 }

 void readLightValue() {
	lightvalues[l] = map(readADC(LDR_PIN), 0, 1023, 0, 100);
	l = (l >= MAX_LDR_READINGS) ? 0 : l + 1;
 }
 
 float calculateAverageTemperature() {
	 float total = 0.0;
	 //int validReadings = 0;

	 for (int i = 0; i < MAX_TMP_READINGS; i++) {
		total += temperatures[i];
	 }

	 return total / MAX_TMP_READINGS;
 }
 
 float calculateAverageLightIntensity() {
	 float total = 0.0;
	 
	 for (int i = 0; i < MAX_LDR_READINGS; i++) {
		 total = total + lightvalues[i];
	 }
	 
	 return total / MAX_LDR_READINGS;
 }

 void sendStatusUpdate() {

	// PRINT THE VALUES FOR DEBUGGING
	char avg_temperatureS[10];
	dtostrf(calculateAverageTemperature(), 2, 2, avg_temperatureS);
	printf("Average temperature: %s degrees C\n", avg_temperatureS);

	// send temperature
	//transmit16(concat(TMP36,avg_temperature));
	
	char avg_lightS[10];
	dtostrf(calculateAverageLightIntensity(), 2, 2, avg_lightS);
	printf("Average light value: %s%%\n", avg_lightS);
	
	// send lightintensity
	//transmit16(concat(LDR, avg_lightintensity));
	
	// send shutter state
	//transmit16(concat(SHUTTER,shutter_state));
 }