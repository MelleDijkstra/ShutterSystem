/*
 * eeprom.h
 *
 * Created: 8-11-2017 12:03:30
 *  Author: Gebruiker
 */

#ifndef EEPROM_H_
#define EEPROM_H_

#define TEMPADDR 0
#define LIGHTADDR 1
#define MAXDOWNDIST 2
#define MAXUPDIST 3
#define TEMPUPPER 4
#define TEMPLOWER 5
#define LIGHTUPPER 6
#define LIGHTLOWER 7

void saveMaxTmp(uint8_t tmp);
void saveMaxDownDist(uint8_t dist);
void saveMaxUpDist(uint8_t dist);
void saveMaxTemp(uint8_t tmp);
void saveMinTemp(uint8_t tmp);
void saveMaxLight(uint8_t light);
void saveMinLight(uint8_t light);

uint8_t loadMaxTmp();
uint8_t loadMaxDownDist();
uint8_t loadMaxUpDist();
uint8_t loadMinTemp();
uint8_t loadMaxLight();
uint8_t loadMinLight();

#endif /* EEPROM_H_ */