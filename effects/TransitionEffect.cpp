#include "TransitionEffect.h"
#include "../Utils.h"
#include "../debug.h"

void TransitionEffect::setTranstion(CRGB from, CRGB to) {
	for (int i = 0; i < NUMBER_OF_LEDS; i++) {
		startColor[i] = from;
		endColor[i] = to;
		step[i] = 0;
	}
}

CRGB TransitionEffect::calculatePixelValue(int i) {

	CRGB result;

	uint8_t s = step[i];
	uint8_t r1 = startColor[i].r;
	uint8_t r2 = endColor[i].r;
	uint8_t g1 = startColor[i].g;
	uint8_t g2 = endColor[i].g;
	uint8_t b1 = startColor[i].b;
	uint8_t b2 = endColor[i].b;

	if (s > 0) {
		result.r = lerp8by8(r1, r2, s);
		result.g = lerp8by8(g1, g2, s);
		result.b = lerp8by8(b1, b2, s);
	}
	/*
	 printColor(startColor[i], "start");
	 printColor(endColor[i], "end");
	 printColor(result, "result");
	 */
	return result;

}

/**
 * Calculates the colors from the start-endcolors using steps into the rawLEDBuffer
 */
void TransitionEffect::updateRawLEDBuffer() {
	for (int i = 0; i < NUMBER_OF_LEDS; i++) {
		ledBuffer[i] = calculatePixelValue(i);
	}
}

/**
 * Sets the color transition.
 * i - pixel index
 * color - next color for the index
 */
void TransitionEffect::setColorTransition(int i, CRGB color) {
	//printColor(color, "new color transition");
	startColor[i] = calculatePixelValue(i);
	endColor[i] = color;
	step[i] = 0;
}

void TransitionEffect::radiiTransition(int startIndex, bool includeCenter,
		CRGB color) {
	if (startIndex <= 10) {
		if (includeCenter) {
			setColorTransition(0, color);
		}
		for (int i = startIndex; i < NUMBER_OF_LEDS; i += 10) {
			setColorTransition(i, color);
		}
	}
}

void TransitionEffect::starTransition(int starIndex, CRGB color) {
	uint8_t start = 0;
	uint8_t end = 0;
	if (starIndex < 4) {
		switch (starIndex) {
		case (0):
			break;
		case (1):
			start = 1;
			end = 10;
			break;
		case (2):
			start = 11;
			end = 20;
			break;
		case (3):
			start = 21;
			end = 30;
			break;

		}
		for (int i = start; i <= end; i++) {
			setColorTransition(i, color);
		}
	}
}

void TransitionEffect::updateTransition() {
	for (int i = 0; i < NUMBER_OF_LEDS; i++) {
		step[i] = qadd8(step[i], 1);
	}
}

CRGB TransitionEffect::getColor() {
	CRGB color;
	switch (this->color) {
	case (COLOR_RANDOM):
		color = randomColor();
		break;
	case (COLOR_HSV):
		color = CHSV(frame, 255, 255);
	}
	return color;
}

void TransitionEffect::dotEffect() {

	if (frame % 100 == 0) {
		int dotIndex;
		switch (style) {
		case STYLE_RANDOM:
			dotIndex = random(NUMBER_OF_LEDS);
			break;
		case STYLE_CONTINOUS_UP:
			if (index >= NUMBER_OF_LEDS) {
				index = 0;
			}
			dotIndex = index++;
			break;
		case STYLE_CONTINOUS_DOWN:
			if (index < 0) {
				index = NUMBER_OF_LEDS - 1;
			}
			dotIndex = index--;
			break;
		}

		setColorTransition(dotIndex, getColor());
	}

	updateRawLEDBuffer();
	updateTransition();

	frame++;

}

void TransitionEffect::updateEffect() {
	switch (type) {
	case (DOT):
		dotEffect();
		break;
	case (RADII):
		radiiEffect();
		break;
	case (STAR):
		starEffect();
		break;
	}
}

void TransitionEffect::initEffect(CRGB rawLEDBuffer[],
		TRANSITION_EFFECT_TYPE type, TRANSITION_EFFECT_STYLE style,
		TRANSITION_EFFECT_COLOR color) {
	this->type = type;
	this->style = style;
	this->color = color;
	this->index = 0;
	ledBuffer = rawLEDBuffer;
	//fillRawLEDBufferWith(CRGB::Black);
	frame = 0;
	TransitionEffect::setTranstion(CRGB::Black, CRGB::Black);
}

void TransitionEffect::radiiEffect() {

	//Serial.print("radiiIndex: ");
	//Serial.println(radiiIndex);

	if (frame % 300 == 0) {

		int radiiIndex;
		switch (style) {
		case STYLE_RANDOM:
			radiiIndex = random(1, 11);
			break;
		case STYLE_CONTINOUS_UP:
			if (index > 10) {
				index = 1;
			}
			radiiIndex = index++;

			break;
		case STYLE_CONTINOUS_DOWN:
			if (index <= 1) {
				index = 10;
			}
			radiiIndex = index--;

			break;
		}

		radiiTransition(radiiIndex, true, getColor());
	}

	updateRawLEDBuffer();
	updateTransition();

	frame++;
}

void TransitionEffect::starEffect() {

	if (frame % 300 == 0) {
		int starIndex;
		switch (style) {
		case STYLE_RANDOM:
			starIndex = random(0, 4);
			break;
		case STYLE_CONTINOUS_UP:
			if (index > 3) {
				index = 0;
			}
			starIndex = index++;
			break;
		case STYLE_CONTINOUS_DOWN:
			if (index < 0) {
				index = 3;
			}
			starIndex = index--;
			break;
		}

		starTransition(starIndex, getColor());
	}

	updateRawLEDBuffer();
	updateTransition();

	frame++;
}
