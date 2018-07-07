/*
 * LUMUEffect.cpp
 *
 *  Created on: 2017. dec. 22.
 *      Author: Acer
 */

#include "LUMUEffect.h"

void LUMUEffect::updateEffect() {
	CRGB color;

	unsigned int frac = phase << LUMU_MAX_PHASE_SHIFT;
	//Serial.println(phase);

	color.r = lerp16by16(red, nextRed, frac);
	color.g = lerp16by16(green, nextGreen, frac);
	color.b = lerp16by16(blue, nextBlue, frac);
	/* Serial.print(color.r);
	 Serial.print("/");
	 Serial.print(color.g);
	 Serial.print("/");
	 Serial.println(color.b);
	 */
	for (int i = 0; i < NUMBER_OF_LEDS; i++) {
		leds[i] = color;
	}

	for (int i=0; i<NUMBER_OF_SPARKS; i++) {
		leds[sparkIndex[i]] = CRGB::White;
	}
	sparkPhase++;
	if (sparkPhase > MAX_SPARK_PHASE) {
		sparkPhase = 0;
		for (int i=0; i<NUMBER_OF_SPARKS; i++) {
			sparkIndex[i] = random16(NUMBER_OF_LEDS);
		}
	}

	phase += 1;

	if (phase >= LUMU_MAX_PHASE) {

		phase = 0;

		red = color.r;
		green = color.g;
		blue = color.b;

		//CHSV hsv = CHSV(random(256), 255, 127);
		//CRGB rgb;
		//hsv2rgb_rainbow(hsv, rgb);
		paletteIndex++;
		if (paletteIndex >= LUMU_COLORS_LENGTH) {
			paletteIndex = 0;
		}

		nextRed = LUMU_COLORS[paletteIndex].r;
		nextGreen = LUMU_COLORS[paletteIndex].g >> 2;
		nextBlue = LUMU_COLORS[paletteIndex].b >> 2;

	}
}

void LUMUEffect::initEffect(CRGB rawLEDBuffer[]) {
	for (int i=0; i<NUMBER_OF_SPARKS; i++) {
		sparkIndex[i] = random16(NUMBER_OF_LEDS);
	}
	sparkPhase = 0;

	leds = rawLEDBuffer;

	paletteIndex = 0;
	phase = 0;

	red = 0x00;
	green = 0x00;
	blue = 0x00;

	nextRed = LUMU_COLORS[paletteIndex].r;
	nextGreen = LUMU_COLORS[paletteIndex].g;
	nextBlue = LUMU_COLORS[paletteIndex].b;

	stopFlag = false;
}

unsigned long LUMUEffect::sumLeds() {
	unsigned long result = 0;
	for (int i = 0; i < NUMBER_OF_LEDS; i++) {
		/*   Serial.print(i);
		 Serial.print(" : ");
		 Serial.print(leds[i].r);
		 Serial.print(",");

		 Serial.print(leds[i].g);
		 Serial.print(",");
		 Serial.print(leds[i].b);
		 Serial.println();
		 */
		result += leds[i].r;
		result += leds[i].g;
		result += leds[i].b;
	}
	return result;
}

