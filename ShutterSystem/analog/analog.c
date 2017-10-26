/*
 * analog.c
 *
 * Created: 24-10-2017 13:59:34
 *  Author: melle
 */ 

#include <avr/io.h>

void initADC() {
	// select Vref = AVcc because we are using 5V
	ADMUX |= (1<<REFS0);
	// set prescaller to 128 and enable ADC
	ADCSRA |= (1<<ADEN) | (1<<ADPS0) | (1<<ADPS1) | (1<<ADPS2);
}

// reads analog input from given channel and returns value between 0 - 1023
uint16_t readADC(uint8_t channel) {
	//select ADC channel with safety mask
	ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);
	// set start conversion bit in status register
	ADCSRA |= (1<<ADSC);
	// loop until conversion is finished (ADSC bit in status register is cleared)
	loop_until_bit_is_clear(ADCSRA,ADSC);
	// return analog value
	return ADC;
}