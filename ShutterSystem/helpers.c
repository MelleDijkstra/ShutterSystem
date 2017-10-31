/*
 * helpers.c
 *
 * Created: 26-10-2017 22:05:02
 *  Author: melle
 */ 

 #include <stdint.h>
 #include <stdlib.h>

 // map a range into another range
 // source: arduino lib 
 // TODO: add url
 long map(long x, long in_min, long in_max, long out_min, long out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
 }

 // concatenate 2 bytes with each other
 uint16_t concat(uint8_t high, uint8_t low)
 {
	 return (high << 8) | low;
 }
