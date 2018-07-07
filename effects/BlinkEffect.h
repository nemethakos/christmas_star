/*
 * BlinkEffect.h
 *
 *  Created on: 2017. dec. 9.
 *      Author: Acer
 */

#ifndef EFFECTS_BLINKEFFECT_H_
#define EFFECTS_BLINKEFFECT_H_

#include "../BoardSettings.h"
#include "FastLED.h"

class BlinkEffect {
	CRGB* ledBuffer;
	unsigned long frame;
	bool  on;
public:
	/**
	 * Returns the name of the effect
	 */
	char* getName();


	void initEffect(CRGB rawLEDBuffer[]);

	/**
	 * updates the effect
	 */
	void updateEffect();

};

#endif /* EFFECTS_BLINKEFFECT_H_ */
