#ifndef EFFECTS_STAREFFECT_H_
#define EFFECTS_STAREFFECT_H_
#include "FastLED.h"

class StarEffect {
	int index;
	bool direction;
	CRGB* rawLEDBuffer;
	unsigned long frame;
public:
	char* getName();

	void drawStar(int layer, CRGB color);

	/**
	 * Performs the necessary initialization
	 */
	void initEffect(CRGB rawLEDBuffer[]);


	/**
	 * updates the effect
	 */
	void updateEffect();
};

#endif /* EFFECTS_STAREFFECT_H_ */
