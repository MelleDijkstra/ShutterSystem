/*
 * analog.h
 *
 * Created: 24-10-2017 14:00:38
 *  Author: melle
 */ 


#ifndef ANALOG_H_
#define ANALOG_H_

#include <stdint.h>

void initADC();
uint16_t readADC(uint8_t);

#endif /* ANALOG_H_ */