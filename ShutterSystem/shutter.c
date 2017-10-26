/*
 * shutter.c
 *
 * Created: 26-10-2017 16:02:35
 *  Author: melle
 */ 

 #include <stdbool.h>
 #include <stdint.h>
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
 uint16_t lightintensity = 0;
 bool shutter_state = false;

 float readTemperature() {
	uint16_t reading = readADC(TMP_PIN);

	float voltage = (reading * 5.0) / 1024.0;
	temperature = (voltage - 0.5) * 100;

	return temperature;
 }

 uint16_t readLightValue() {
	lightintensity = readADC(LDR_PIN);
	return lightintensity;
 }

 void sendStatusUpdate() {
	// send temperature
	transmit16(concat(TMP36,temperature));
	// send lightintensity
	transmit16(concat(LDR,lightintensity));
	// send shutter state
	transmit16(concat(SHUTTER,shutter_state));
 }