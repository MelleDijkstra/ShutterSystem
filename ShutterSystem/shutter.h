/*
 * shutter.h
 *
 * Created: 26-10-2017 16:03:40
 *  Author: melle
 */ 


#ifndef SHUTTER_H_
#define SHUTTER_H_

#define MAX_TMP_READINGS 40
#define MAX_LDR_READINGS 30

void readLightValue();
void readTemperature();
float calculateAverageTemperature();
float calculateAverageLightIntensity();

void sendStatusUpdate();

#endif /* SHUTTER_H_ */