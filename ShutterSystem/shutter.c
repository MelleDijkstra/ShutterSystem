/*
 * shutter.c
 *
 * Created: 26-10-2017 16:02:35
 *  Author: melle
 */ 

 #include <stdbool.h>

 #define TMP_PIN 0 // TMP36 pin is on A0
 #define LDR_PIN 1 // LDR pin is on A1

 // ID's are used for serial communication
 #define TMP36		1 
 #define LDR		2 
 #define SHUTTER	3 

 uint8_t temperature = 0;
 uint8_t lightintensity = 0;
 bool shutter_state = false;

 uint8_t readLightValue() {
	temperature = readADC(TMP_PIN);
	return temperature;
 }

 uint8_t readTempValue() {
	lightintensity = readADC(LDR_PIN);
	return lightintensity;
 }

 void sendStatusUpdate() {
	// send temperature
	transmit(TMP36);
	transmit(temperature);
	// send lightintensity
	transmit(LDR);
	transmit(lightintensity);
	// send shutter state
	transmit(SHUTTER);
	transmit(shutter_state);
 }