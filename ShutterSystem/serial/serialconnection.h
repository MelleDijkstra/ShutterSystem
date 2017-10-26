/*
 * serialconnection.h
 *
 * Created: 24-10-2017 14:39:19
 *  Author: melle
 */ 

#ifndef SERIALCONNECTION_H_
#define SERIALCONNECTION_H_

#include <stdint.h>

void initUART();
void transmit(uint8_t data);
uint8_t receive();

#endif /* SERIALCONNECTION_H_ */