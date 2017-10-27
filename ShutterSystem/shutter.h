/*
 * shutter.h
 *
 * Created: 26-10-2017 16:03:40
 *  Author: melle
 */ 


#ifndef SHUTTER_H_
#define SHUTTER_H_

void readLightValue();
void readTemperature();
void calculateAverageTemperature();
void calculateAverageLightIntensity();

void sendStatusUpdate();

#endif /* SHUTTER_H_ */