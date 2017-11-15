/*
 * shutter.h
 *
 * Created: 26-10-2017 16:03:40
 *  Author: melle
 */ 

#ifndef SHUTTER_H_
#define SHUTTER_H_

#define MAX_TMP_READINGS 40
#define MAX_LDR_READINGS 30

enum state {
	UP,
	DOWN,
	PROGRESS_UP,
	PROGRESS_DOWN
};

typedef struct
{
	uint8_t maxTmp;
	uint8_t	minTmp;
	uint8_t	maxDownDist;
	uint8_t	maxUpDist;
	uint8_t	maxLight;
	uint8_t	minLight;
} settings;

void initShutter();
void readLightValue();
void readTemperature();
float calculateAverageTemperature();
float calculateAverageLightIntensity();

void roll();
void emulateRoll();

void sendStatusUpdate();
void controllerInputInterrupt(uint8_t byte);
void processMessage(uint8_t msg[2]);

#endif /* SHUTTER_H_ */