/*
 * distance.c
 *
 * meausers distance betwee 2 and 70 cm
 *
 * Created: 29-6-2016 14:44:43
 *  Author: jacob
 */ 

/* 
 * HC-SR04
 * trigger to sensor : uno 0 (PD0) output
 * echo from sensor  : uno 3 (PD3 = INT1) input
 *
 */

 #define F_CPU 16E6

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include "distance.h"
#include "serial/serialconnection.h"
#include "io/io.h"

#define TRIG 2
#define ECHO 3

volatile uint16_t gv_counter = 0; // 16 bit counter value
volatile uint8_t gv_echo = BEGIN; // a flag

void init_timer(void)
{
	// prescaling : max time = 2^16/16E6 = 4.1 ms, 4.1 >> 2.3, so no prescaling required
	// normal mode, no prescale, stop timer
    TCCR1A = 0;
    TCCR1B = 0;
}

void init_ext_int(void)
{
    // any change triggers ext interrupt 1
    EICRA = (1 << ISC10);
    EIMSK = (1 << INT1);
}

uint16_t calc_cm(uint16_t counter)
{
	uint16_t micro_sec = counter / 16;
    return (micro_sec / 58.2);
}

//set stream pointer
FILE usart0_str = FDEV_SETUP_STREAM(transmitChar, NULL, _FDEV_SETUP_WRITE);

volatile uint8_t BusyMeasure = 0;

void Fire_Sensor()
{
	setPin(TRIG, HIGH);
	// PORTD |= (1<<TRIG);				// Set TRIG pin high for 10us to start measure.
	BusyMeasure = 1;               // We going to start new measure, so sensor is busy !
	_delay_us(15);					// 10us Delay.
	setPin(TRIG, LOW);
	// PORTD &= ~(1<<TRIG);			// Set TRIG pin LOW.
}

int main(void)
{
	// initialize serial connection
	initUART();
	stdout = &usart0_str;

	// INT1 Use/Configuration
	inputPin(ECHO);
	// DDRD &= ~(1<<INT1_pin);		// Set PD2 as input (Using for interrupt INT1).
	setPin(ECHO, HIGH);
	// PORTD |= (1<<INT1_pin);		// Enable PD2 pull-up resistor.
	EICRA |= (1<<ISC10);
	EIMSK |= (1<<INT1);
	// GICR  |= (1<<INT1);			// Enable INT1 interrupts.
	// MCUCR |= (1<<ISC10) ;		// Trigger INT1 on any logic change.
	
	// Trig Conf
	outputPin(TRIG);
	// DDRD |= (1<<Trig);			// Triger pin as output
	setPin(TRIG, LOW);
	//PORTD &= ~(1<<Trig);		// LOW

	// Use/Configuration	 of Timer0 (8 bit timer)
	init_timer();

	sei();										// Global interrupts Enabled.

	while(1) {
		if (BusyMeasure == 0)
		{
			Fire_Sensor();
			_delay_ms(60);
		}
		
		printf("cm: %u, ", calc_cm(gv_counter));
		_delay_ms(1000);
	};

    return 0;
}

ISR (INT1_vect)
{
	if (gv_echo == BEGIN)	// high pulse 0 --> 1 , start time measure
	{
		// set timer value to 0
		TCNT1 = 0;
		// start timer
		TCCR1B = (1<<CS10);
		gv_echo = END;
	} else {				// low pulse 1 --> 0 , result is ready
		// get timer value how long it took to receive ECHO
		gv_counter = TCNT1;
		// stop timer
		TCCR1B = 0;
		BusyMeasure = 0;
		gv_echo = BEGIN;
	}
}
