/*
 * ShutterSystem.c
 *
 * Created: 20-10-2017 17:41:13
 * Author : melle
 */ 

//#include <avr/io.h>
//#include "scheduler/scheduler.h"
//#include "shutter.h"
//
//int main() {
//
	//// initialize serial communication
	//initUART();
	//// initialize Analog to Digital converter for reading TMP36 and LDR
	//initADC();
	//// initialize scheduler
	//initSCH();
	//
	//// Add tasks that need to be executed
	//SCHAddTask(readTempValue, 0, (1000 * 40)); // read temperature every 40 seconds
	//SCHAddTask(readLightValue, 0, (1000 * 30)); // read light every 30 seconds
	//SCHAddTask(sendStatusUpdate, 0, (1000 * 60)); // send status update every 1 minute
//
	//SCHStart();
//
	//while(1) {
		//SCHDispatchTasks();
	//}
//
	//return 0;
//}

#include <stdio.h>
#include <avr/io.h>
#include <stdlib.h>
#define F_CPU 16E6
#include <util/delay.h>
#include "analog/analog.h"
#include "shutter.h"
#include "helpers.h"

#define UBRR_VALUE 51
#define VREF 5

void USART0Init(void)
{
	// Set baud rate
	UBRR0H = (uint8_t)(UBRR_VALUE>>8);
	UBRR0L = (uint8_t)UBRR_VALUE;
	// Set frame format to 8 data bits, no parity, 1 stop bit
	UCSR0C |= (1<<UCSZ01)|(1<<UCSZ00);
	//enable transmission and reception
	UCSR0B |= (1<<RXEN0)|(1<<TXEN0);
}

int USART0SendByte(char u8Data, FILE *stream)
{
	if(u8Data == '\n')
	{
		USART0SendByte('\r', stream);
	}
	//wait while previous byte is completed
	while(!(UCSR0A&(1<<UDRE0))){};
	// Transmit data
	UDR0 = u8Data;
	return 0;
}

//set stream pointer
FILE usart0_str = FDEV_SETUP_STREAM(USART0SendByte, NULL, _FDEV_SETUP_WRITE);

int main()
{
	
	float tmp;

	//initialize ADC
	initADC();
	//Initialize USART0
	USART0Init();
	//assign our stream to standard I/O streams
	stdout = &usart0_str;

	while(1)
	{
		tmp = readTemperature();
		
		char temperatureS[10];
		dtostrf(tmp, 2, 2, temperatureS);
		printf("%s degrees C\n", temperatureS);

		uint16_t light = readLightValue();
		printf("light value: %lu%%\n", map(light, 0, 1023, 0, 100));

		_delay_ms(500);
	}
}
