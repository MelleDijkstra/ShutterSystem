/*
 * io.c
 *
 * Created: 28-10-2017 22:18:14
 *  Author: melle
 */ 

 // For the bitwise operations the following source is used
 // https://stackoverflow.com/questions/47981/how-do-you-set-clear-and-toggle-a-single-bit

 #include <stdint.h>
 #include <avr/io.h>

 void inputPin(uint8_t pin) {
	// check which ports to use
	if(pin < 8) {
		DDRD &= ~(1<<pin);
	} else if (pin <= 13) {
		pin -= 8;
		DDRB &= ~(1<<pin); 
	}
	// if pin is higher then 13 then do nothing...
 }

 void outputPin(uint8_t pin) {
	// check which ports to use
	if(pin < 8) {
		DDRD |= (1<<pin);
	} else if (pin <= 13) {
		pin -= 8;
		DDRB |= (1<<pin);
	}
	// if pin is higher then 13 then do nothing...
 }

 void digitalWrite(uint8_t pin, uint8_t state) {
	state &= 1; // make sure the state is 0 or 1
	// check which ports to use
	if(pin < 8) {
		PORTD ^= (-state ^ PORTD) & (1 << pin);
	} else if (pin <= 13) {
		pin -= 8;
		PORTB ^= (-state ^ PORTB) & (1 << pin);
	}
	// if pin is higher then 13 then do nothing...
 }