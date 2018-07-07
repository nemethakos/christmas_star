/*
 * Transition.h
 *
 *  Created on: 2017. dec. 9.
 *      Author: Acer
 */

#ifndef EFFECTS_TRANSITIONEFFECT_H_
#define EFFECTS_TRANSITIONEFFECT_H_

#include "../BoardSettings.h"
#include "FastLED.h"
#include <stdint.h>

enum TRANSITION_EFFECT_COLOR {
	COLOR_RANDOM,
	COLOR_HSV
};

enum TRANSITION_EFFECT_STYLE {
	/**
	 * Random dots/radiis/stars
	 */
	STYLE_RANDOM,
	/**
	 * dots from 0 to 30, radiis from start index 1 to 10,
	 * stars from 0 to 4
	 */
	STYLE_CONTINOUS_UP,
	/**
	 * Same as UP but reverse direction
	 */
	STYLE_CONTINOUS_DOWN
};

enum TRANSITION_EFFECT_TYPE {
	DOT, RADII, STAR
};

class TransitionEffect {
	CRGB* ledBuffer;
	unsigned long frame;
	CRGB startColor[NUMBER_OF_LEDS];
	CRGB endColor[NUMBER_OF_LEDS];
	uint8_t step[NUMBER_OF_LEDS];
	// wait before transition
	//int8_t wait[NUMBER_OF_LEDS];
	TRANSITION_EFFECT_TYPE type;
	TRANSITION_EFFECT_STYLE style;
	TRANSITION_EFFECT_COLOR color;
	int8_t index;
public:
	TransitionEffect();
	TransitionEffect(const TransitionEffect& other);
	void radiiTransition(int startIndex, bool includeCenter, CRGB color);
	void starTransition(int starIndex, CRGB color);
	void initEffect(CRGB rawLEDBuffer[], TRANSITION_EFFECT_TYPE type,
			TRANSITION_EFFECT_STYLE style, TRANSITION_EFFECT_COLOR color);
	void setTranstion(CRGB from, CRGB to);
	CRGB calculatePixelValue(int i);
	void updateRawLEDBuffer();
	void updateTransition();
	void setColorTransition(int i, CRGB color);
	void updateEffect();
	void dotEffect();
	void radiiEffect();
	void starEffect();
	CRGB getColor();
};

#endif /* EFFECTS_TRANSITIONEFFECT_H_ */
