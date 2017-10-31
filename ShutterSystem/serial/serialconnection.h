/*
 * serialconnection.h
 *
 * Created: 24-10-2017 14:39:19
 *  Author: melle
 */ 

#ifndef SERIALCONNECTION_H_
#define SERIALCONNECTION_H_

#include <stdint.h>

void setTrigger(void (*)(uint8_t));
void initUART();
void transmit8(uint8_t);
void transmit16(uint16_t);
//void sendstr(char* str);
int transmitChar(char data, FILE *stream);
uint8_t receive();

#endif /* SERIALCONNECTION_H_ */