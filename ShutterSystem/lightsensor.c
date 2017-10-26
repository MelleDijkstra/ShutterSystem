/*
 * lightsensor.c
 *
 * Created: 26-10-2017 11:37:52
 *  Author: Gebruiker
 */ 
 #include <avr/io.h>
 #define F_CPU 16E6
 #include <util/delay.h>
 #include "serial/serialconnection.h"

 int main (void)
 {
	 initADC();
	 initUART();

	 while(1)
	 {

		 uint8_t adc_value = readADC(); // read A0 pin and invert the result

		 transmit(adc_value);
		 // mV * 5v
		 float voltage = adc_value * 5.0;
		 //convert mV to Voltage
		 voltage /= 1024.0;

		 //light from 0-100%
		 uint8_t lightpercentage = voltage / 5 * 100;

		 transmit(lightpercentage);

		 _delay_ms(1000);
	 }

 }
 
