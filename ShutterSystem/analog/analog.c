/*
 * analog.c
 *
 * Created: 24-10-2017 13:59:34
 *  Author: melle
 */ 

#include <avr/io.h>

void initADC() {
	// TODO: comments
	ADMUX = (1 << REFS0) | (1 << ADLAR);
	ADCSRA = (1 << ADEN) | (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);
}

uint8_t readADC(uint8_t channel) {
	// set start conversion bit in status register
	ADCSRA |= (1 << ADSC);
	// loop until conversion is finished (ADSC bit in status register is cleared)
	loop_until_bit_is_clear(ADCSRA,ADSC);
	// return analog value
	return ADCH;
}