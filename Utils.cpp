#include "Utils.h"

void clearBuf(CRGB rawLEDBuffer[]) {
	for (int i = 0; i < NUMBER_OF_LEDS; i++) {
		rawLEDBuffer[i] = 0;
	}
}



CRGB randomColor() {
	CRGB result;

	result.r = random(256);
	result.g = random(256);
	result.b = random(256);

	return result;
}
