/*
 * shutter.c
 *
 * Created: 26-10-2017 16:02:35
 *  Author: melle
 */ 

 #include <stdbool.h>
 #include <stdint.h>
 #include <stdio.h>
 #include "analog/analog.h"
 #include "serial/serialconnection.h"
 #include "helpers.h"

 #define TMP_PIN 0 // TMP36 pin is on A0
 #define LDR_PIN 1 // LDR pin is on A1

 // ID's are used for serial communication
 #define TMP36		1 
 #define LDR		2 
 #define SHUTTER	3 

 float temperature = 0;
 static float temperatures[40];
 static uint16_t lightvalues[30];
 float avg_lightintensity;
 float avg_temperature;		

 uint16_t lightintensity = 0;
 int t = 0;
 int l = 0;
 bool shutter_state = false;

 void readTemperature() {
	uint16_t reading = readADC(TMP_PIN);
	char temperatureS[10];

	float voltage = (reading * 5.0) / 1024.0;
	temperature = (voltage - 0.5) * 100;
	dtostrf(temperature, 2, 2, temperatureS);
	printf("%s degrees C\n", temperatureS);
	temperatures[t] = temperature;
	t++;	
 }

 void readLightValue() {
	lightintensity = readADC(LDR_PIN);	
	lightvalues[l] = lightintensity;
	l++;
	printf("Light value: %lu%%\n", map(lightintensity, 0, 1023, 0, 100));
 }
 
 void calculateAverageTemperature() {				
	 float temp = 0.0;		
	 int i;		
	 
	 for ( i = 0; i < 40; i++ ) {
		 temp = temp + temperatures[i];
	 }
	 
	 avg_temperature = temp / 40.0;
 }
 
 void calculateAverageLightIntensity() {				
	 float light = 0.0; 
	 int i;			
	 for ( i = 0; i < 30; i++ ) {
		 light = light + lightvalues[i];
	 }
	 
	 avg_lightintensity = light / 30.0;
 }

 void sendStatusUpdate() {
	 char avg_temperatureS[10];
	
	// send temperature
	dtostrf(avg_temperature, 2, 2, avg_temperatureS);
	printf("Average temperature: %s degrees C\n", avg_temperatureS);
	//transmit16(concat(TMP36,avg_temperature));
	
	// send lightintensity
	printf("Average light value: %lu%%\n", map(avg_lightintensity, 0, 1023, 0, 100));
	// transmit16(concat(LDR, avg_lightintensity));
	
	// send shutter state
	transmit16(concat(SHUTTER,shutter_state));
 }