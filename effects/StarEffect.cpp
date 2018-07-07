#include "StarEffect.h"
#include "../Utils.h"

char* StarEffect::getName() {
	return (char*) "Star";
}

void StarEffect::drawStar(int layer, CRGB color) {

	int start = 0;
	int end = 1;

	if (layer == 0) {
	} else if (layer == 1) {
		start = 1;
		end = 11;
	} else if (layer == 2) {
		start = 11;
		end = 21;
	} else {
		start = 21;
		end = 31;
	}
	for (int i = start; i < end; i++) {
		rawLEDBuffer[i] = color;
	}

}

/**
 * updates the effect
 */
void StarEffect::updateEffect() {
	if (frame == 200) {

		drawStar(random(3), randomColor());
		frame = 0;
	}

	if (direction) {
		index++;
		if (index > 2) {
			direction = false;
		}
	} else {
		index--;
		if (index < 1) {
			direction = true;
		}
	}
	frame++;
}

/**
 * Performs the necessary initialization
 */
void StarEffect::initEffect(CRGB rawLEDBuffer[]) {
	this->rawLEDBuffer = rawLEDBuffer;
	this->frame = 0;
}

