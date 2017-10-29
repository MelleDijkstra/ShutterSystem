/*
 * serialconnection.c
 *
 * Created: 24-10-2017 14:54:21
 *  Author: melle
 */ 

#include <stdio.h>
#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>

// output on USB = PD1 = board pin 1
// datasheet p.190; F_OSC = 16 MHz & baud rate = 19.200
#define UBRRVAL 51

// the function pointer to listener
void (*trigger)(uint8_t byte);

// sets the trigger to given function to be notified about received data
void setTrigger(void (*trig)(uint8_t)) {
	trigger = trig;
}

void initUART()
{
	// set the baud rate
	UBRR0H = (uint8_t)(UBRRVAL>>8);
	UBRR0L = (uint8_t)UBRRVAL;
	// disable U2X mode
	UCSR0A = 0;
	// set frame format : asynchronous, 8 data bits, 1 stop bit, no parity
	UCSR0C |= (1<<UCSZ01) | (1<<UCSZ00);
	// enable transmission and reception. Also work with interrupts RXCIE0
	UCSR0B |= (1<<RXEN0) | (1<<TXEN0) | (1 << RXCIE0);
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
	uint8_t high_byte = (uint8_t)(data>>8); // get high byte
	uint8_t low_byte = data & 0xff; // get low byte
	// send the 2 bytes
	transmit8(high_byte); // first send high
	transmit8(low_byte);  // then send low
}

// void sendstr(char* str) {
// 	int len = strlen(str);
// 	for (int c = 0; c < len;c++)
// 	{
// 		if (str[c] == '\n')
// 		{
// 			transmit8('\r');
// 		}
// 		transmit8(str[c]);
// 	}
// }

uint8_t receive() {
	// Wait for data to be received
	loop_until_bit_is_set(UCSR0A, RXC0);
	// Get and return received data from buffer
	return UDR0;
}

int transmitChar(char data, FILE *stream)
{
	if(data == '\n')
	{
		transmitChar('\r', stream);
	}
	// transmit data
	transmit8(data);
	return 0;
}

// This interrupt runs when data is received
ISR(USART_RX_vect) {
	// notify the trigger that data is received
	trigger(UDR0);
}
