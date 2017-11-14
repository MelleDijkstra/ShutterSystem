/*
 * eeprom.c
 *
 * Created: 8-11-2017 11:29:10
 *  Author: Thijs
 */ 
#include <avr/eeprom.h>
#include <avr/io.h>
#include "eeprom.h"
#include <stdint.h>

void saveMaxTmpSetting(uint8_t tmp) {
	eeprom_write_byte((uint8_t*)TEMPUPPER, tmp);
}

void saveMaxDownDist(uint8_t dist) {
	eeprom_write_byte((uint8_t*)MAXDOWNDIST, dist);
}

void saveMaxUpDist(uint8_t dist) {
	eeprom_write_byte((uint8_t*)MAXUPDIST, dist);
}

void saveMinTemp(uint8_t tmp) {
	eeprom_write_byte((uint8_t*)TEMPLOWER, tmp);
}

void saveMaxLight(uint8_t light) {
	eeprom_write_byte((uint8_t*)LIGHTUPPER, light);
}

void saveMinLight(uint8_t light) {
	eeprom_write_byte((uint8_t*)LIGHTLOWER, light);
}

uint8_t loadMaxTmpSetting(){
	 return eeprom_read_byte((uint8_t*) TEMPUPPER);
}

uint8_t loadMaxDownDist(){
	return eeprom_read_byte((uint8_t*) MAXDOWNDIST);
}

uint8_t loadMaxUpDist(){
	return eeprom_read_byte((uint8_t*) MAXUPDIST);
}

uint8_t loadMinTemp(){
	return eeprom_read_byte((uint8_t*) TEMPLOWER);
}

uint8_t loadMaxLight(){
	return eeprom_read_byte((uint8_t*) LIGHTUPPER);
}

uint8_t loadMinLight(){
	return eeprom_read_byte((uint8_t*) LIGHTLOWER);
}

//uint8_t max_tmp_setting = loadMaxTmpSetting();