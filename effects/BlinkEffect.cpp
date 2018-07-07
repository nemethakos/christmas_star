/*
 * BlinkEffect.cpp
 *
 *  Created on: 2017. dec. 9.
 *      Author: Acer
 */

#include "BlinkEffect.h"

char* BlinkEffect::getName() {
	return (char*) "Blink";
}
/**
 * Performs the necessary initialization
 */

void BlinkEffect::initEffect(CRGB rawLEDBuffer[])
{
	ledBuffer = rawLEDBuffer;
	frame = 0;
	on = false;
}

/**
 * updates the effect
 */
void BlinkEffect::updateEffect() {
	if (frame == 1000) {
		on = !on;
		frame = 0;
	}
	CRGB color;
	if (on) {
		color = CRGB::Blue;
	} else {
		color = CRGB::Black;
	}
	for (int i = 0; i < NUMBER_OF_LEDS; i++) {
		ledBuffer[i] = color;
	}
	frame++;
}

