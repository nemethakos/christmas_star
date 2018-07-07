/*
 * FireEffect.h
 *
 *  Created on: 2017. dec. 18.
 *      Author: Acer
 */

#ifndef EFFECTS_FIREEFFECT_H_
#define EFFECTS_FIREEFFECT_H_

#define HEAT_ROW_NUM (4)
#define HEAT_COL_NUM (10)

#include <FastLED.h>

class FireEffect {
	CRGBPalette16 gPal;
	CRGB* ledBuffer;
	unsigned long frame;
	bool gReverseDirection = false;
	uint8_t heat[HEAT_ROW_NUM][HEAT_COL_NUM];
	int mapPixel(int row, int col);
public:
	FireEffect();

	void initEffect(CRGB rawLEDBuffer[]);
	int getHeat(int row, int col);
	/**
	 * updates the effect
	 */
	void updateEffect();
	void Fire2012WithPalette();
	CRGB map(uint8_t hotness);
};

#endif /* EFFECTS_FIREEFFECT_H_ */
