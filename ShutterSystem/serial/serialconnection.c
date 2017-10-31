/*
 * serialconnection.c
 *
 * Created: 24-10-2017 14:54:21
 *  Author: melle
 */ 

#include <avr/io.h>
#include <stdlib.h>

// output on USB = PD1 = board pin 1
// datasheet p.190; F_OSC = 16 MHz & baud rate = 19.200
#define UBBRVAL 51

void initUART()
{
	// set the baud rate
	UBRR0H = 0;
	UBRR0L = UBBRVAL;
	// disable U2X mode
	UCSR0A = 0;
	// enable transmitter
	UCSR0B = _BV(TXEN0);
	// set frame format : asynchronous, 8 data bits, 1 stop bit, no parity
	UCSR0C = _BV(UCSZ01) | _BV(UCSZ00);
}

void transmit8(uint8_t data)
{
	// wait for an empty transmit buffer
	// UDRE is set when the transmit buffer is empty
	loop_until_bit_is_set(UCSR0A, UDRE0);
	// send the data
	UDR0 = data;
}

void transmit16(uint16_t data)
{
	uint8_t high_byte = (uint8_t)data >> 8; // get high byte
	uint8_t low_byte = data & 0xff; // get low byte
	// send the 2 bytes
	transmit8(high_byte); // first send high
	transmit8(low_byte);  // then send low
}

uint8_t receive() {
	// Wait for data to be received
	loop_until_bit_is_set(UCSR0A, RXC0);
	// Get and return received data from buffer
	return UDR0;
}

