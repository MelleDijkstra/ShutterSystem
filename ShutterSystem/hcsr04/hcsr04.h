/*
 * hcsr04.h
 *
 * Created: 10-11-2017 15:26:03
 *  Author: melle
 */ 


#ifndef HCSR04_H_
#define HCSR04_H_

// flags for when to start counting or stop counting
#define BEGIN 0x1
#define END 0x0

// the ECHO and TRIGGER pins, change these according to setup
#define TRIG 2
#define ECHO 3

void initHCSR04();
void fireSensor();
uint16_t calcDistance(uint16_t counter);
void kickEcho();
uint16_t measureDistance();

#endif /* HCSR04_H_ */