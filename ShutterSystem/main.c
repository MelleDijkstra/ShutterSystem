/*
 * ShutterSystem.c
 *
 * Created: 20-10-2017 17:41:13
 * Author : melle
 */ 

#define F_CPU 16E6

#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>
#include "io/io.h"
#include "analog/analog.h"
#include "scheduler/scheduler.h"
#include "serial/serialconnection.h"
#include "helpers.h"
#include "shutter.h"

#define LED 13

//set stream pointer
FILE usart0_str = FDEV_SETUP_STREAM(transmitChar, NULL, _FDEV_SETUP_WRITE);

void toggleLed() {
	static uint8_t flag = LOW;
	if (flag == LOW) {
		setPin(LED, HIGH);
		flag = HIGH;
	} else {
		setPin(LED, LOW);
		flag = LOW;
	}
}

void heartbeat() {
	static int tick = 1;
	printf("%u bonk bonk\n", tick);
	tick++;
}

int main()
{
	// use debug led to check if scheduler is still running
	outputPin(LED);
	// initialize ADC
	initADC();
	// initialize UART
	initUART();
	// assign our stream to standard I/O streams
	stdout = &usart0_str;
	// initialize scheduler
	initSCH();
	// initialize shutter
	initShutter();

	// scheduler uses period in 10ms, so to get 1 sec. you use 100 to get 1000ms.
	// then to get 60 sec. simply multiply with 60
	// make sure no ADC tasks runs under a second, ADC can't handle that speed

	// every second
// 	SCHAddTask(heartbeat, 0, 100);
	SCHAddTask(readTemperature, 0, 1000);
	SCHAddTask(readLightValue, 0, 1000);
 	SCHAddTask(toggleLed, 0, 1000);
	
	// every 5 seconds
	SCHAddTask(sendStatusUpdate, 0, (5 * 1000));

	//	These loops don't work? Something with ADC that is not working
// 	for(int i = 0; i < MAX_TMP_READINGS;i++)
// 	{
// 		readTemperature();
// 	}
// 
// 	for(int j = 0; j < MAX_LDR_READINGS;j++)
// 	{
// 		readLightValue();
// 	}
// 
// 	printf("DONE CALIBRATING!\n");

	SCHStart();
	// keep dispatching tasks
	while(1) {
		SCHDispatchTasks();
	}
}
