/*
 * hcsr04.c
 *
 * Created: 10-11-2017 15:25:54
 * Author: melle
 */

#define F_CPU 16E6

#include <stdint.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include "hcsr04.h"
#include "../io/io.h"

volatile uint16_t counter = 0; // 16 bit counter value
volatile uint8_t echoFlag = BEGIN; // flag for when to stop or start counting

volatile uint8_t BusyMeasuring = 0; // flag when busy measuring

void initHCSR04() {
	// enables INT1 
	// any change triggers ext interrupt 1
	EICRA = (1 << ISC10);
	EIMSK = (1 << INT1);

	// set ECHO as input pin
	inputPin(ECHO);
	// CHECK IF THIS IS NEEDED!!!!!
	setPin(ECHO, HIGH);
	
	// TRIG is output pin, so it can send burst for the ECHO pin to receive
	outputPin(TRIG);
	setPin(TRIG, LOW);

	// prescaling : max time = 2^16/16E6 = 4.1 ms, 4.1 >> 2.3, so no prescaling required
	// normal mode, no prescale, stop timer
	TCCR1A = 0;
	TCCR1B = 0;
}

// this function will generate the 8 cycle burst
void fireSensor()
{
	setPin(TRIG, LOW);			// set trig pin LOW, just to be sure
	_delay_ms(1);
	setPin(TRIG, HIGH);			// set TRIG pin high for 10us to start measure
	BusyMeasuring = 1;            // we going to start new measure, so sensor is busy!
	_delay_us(12);				// at least 10us delay
	setPin(TRIG, LOW);			// set TRIG pin LOW
}

uint16_t calcDistance(uint16_t count)
{
	// no prescaler: 1 / 16.000.000 then count / 16
	// prescaler: 8 / 16.000.000 then count / 2
	uint16_t micro_sec = count / 2;
	return (micro_sec / 58.2);
}

// sometimes the HCSR04 keeps the ECHO pin HIGH which results in a continuing 0cm
// even though there is an object in front of it
// this function makes sure the ECHO pin is low and start reading correctly again
void kickEcho() {
	outputPin(ECHO);
	_delay_us(10);
	setPin(ECHO, LOW);
	inputPin(ECHO);
	_delay_ms(10);
}

uint16_t measureDistance() {
	if (BusyMeasuring == 0)
	{
		fireSensor();
		// wait till measure is done
		// this is typically 38ms according to data sheet
		_delay_ms(38);
	}
	
	if(counter == 0) {
		kickEcho();
	}

	return calcDistance(counter);
}

// this interrupt will run when the ECHO pin goes HIGH when receiving a sound burst
// and generate an interrupt when the ECHO pin goes LOW at the same time it took to receive a burst
ISR (INT1_vect)
{
	if (echoFlag == BEGIN)	// high pulse 0 --> 1 , start time measure
	{
		// set timer value to 0
		TCNT1 = 0;
		counter = 0;
		// prescaler = 8
		// clockcycles every second on atmega328p = 16.000.000
		// 65535 is the highest counting value of timer1
		// so every (8 / 16.000.000) the timer counts 1 up to TCNT1
		// start timer prescaler  (8 / 16.000.000) * 65535 = 0.0327675 seconds = 32 ms = 32000 us
		// so this timer can count a period of 32ms
		// the calculation for the distance is: us / 58.2 = X cm
		// so for the furthest distance 32000 / 58.2 = 549.82 cm (which is way more then needed)
		TCCR1B |= (1 << CS11);
		echoFlag = END;
	} else {				// low pulse 1 --> 0 , result is ready
		// get timer value how long it took to receive ECHO
		counter = TCNT1;
		// stop timer
		TCCR1B = 0;
		BusyMeasuring = 0;
		echoFlag = BEGIN;
	}
}
