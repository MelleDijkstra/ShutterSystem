/*
 * tempsensor.c
 *
 * Created: 24-10-2017 13:26:16
 *  Author: melle
 */ 

 #include "analog/analog.h"
 #include "serial/serialconnection.h"
 #include <avr/io.h>
 #define F_CPU 16E6
 #include <util/delay.h>

 int main (void)
 {
	initADC();
	initUART();

	while(1)
	{

		uint8_t adc_value = ~readADC(0); // read A0 pin and invert the result

		transmit(adc_value);
		// mV * 5v
		float voltage = adc_value * 5.0;
		//convert mV to Voltage
		voltage /= 1024.0;

		//voltage - 500mV
		//to celcius * 100
		//uint8_t cTemperature = (getVoltage() - 0.5) * 100;
		uint8_t temperature = (voltage - 0.5) * 100;

		transmit(temperature);

		_delay_ms(1000);
	}

 }