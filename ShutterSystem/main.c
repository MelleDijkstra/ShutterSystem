/*
 * ShutterSystem.c
 *
 * Created: 20-10-2017 17:41:13
 * Author : melle
 */ 

#include <avr/io.h>
#include "scheduler/scheduler.h"
#include "shutter.h"

int main() {

	// initialize serial communication
	initUART();
	// initialize Analog to Digital converter for reading TMP36 and LDR
	initADC();
	// initialize scheduler
	initSCH();
	
	// Add tasks that need to be executed
	SCHAddTask(readTempValue, 0, (1000 * 40)); // read temperature every 40 seconds
	SCHAddTask(readLightValue, 0, (1000 * 30)); // read light every 30 seconds
	SCHAddTask(sendStatusUpdate, 0, (1000 * 60)); // send status update every 1 minute

	SCHStart();

	while(1) {
		SCHDispatchTasks();
	}

	return 0;
}



