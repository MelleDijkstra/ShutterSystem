/*
 * io.h
 *
 * Created: 28-10-2017 22:18:26
 *  Author: melle
 */ 


#ifndef IO_H_
#define IO_H_

#define INPUT	0
#define OUTPUT	1

#define HIGH	1
#define LOW		0

void outputPin(uint8_t pin);
void inputPin(uint8_t pin);

void setPin(uint8_t pin, uint8_t value);

#endif /* IO_H_ */