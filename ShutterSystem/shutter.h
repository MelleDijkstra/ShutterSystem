/*
 * shutter.h
 *
 * Created: 26-10-2017 16:03:40
 *  Author: melle
 */ 


#ifndef SHUTTER_H_
#define SHUTTER_H_

uint16_t readLightValue();
float readTemperature();

void sendStatusUpdate();

#endif /* SHUTTER_H_ */