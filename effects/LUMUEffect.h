#ifndef EFFECTS_LUMUEFFECT_H_
#define EFFECTS_LUMUEFFECT_H_
#include "FastLED.h"
#include "../BoardSettings.h"

const CRGB LUMU_COLORS[] = { 0xFF0000, 0xFF0000, 0x800080, 0x800080, 0x0000FF,
		0x0000FF, 0x008080, 0x008080, 0x00FF00, 0x00FF00, 0x808000, 0x808000 };

#define LUMU_COLORS_LENGTH (sizeof(LUMU_COLORS) / sizeof(CRGB))
#define LUMU_MAX_PHASE_BITS (12) // >= 8
#define LUMU_MAX_PHASE_SHIFT (16-LUMU_MAX_PHASE_BITS)
#define LUMU_MAX_PHASE (((unsigned long)1<<LUMU_MAX_PHASE_BITS)-1) // LUMU maximum phase
#define MAX_SPARK_PHASE (50)
#define NUMBER_OF_SPARKS (3)

class LUMUEffect {
	unsigned long sparkPhase;
	uint8_t sparkIndex[NUMBER_OF_SPARKS];
	unsigned long phase;
	boolean stopFlag;
	byte red;
	byte green;
	byte blue;

	byte nextRed;
	byte nextGreen;
	byte nextBlue;
	unsigned long paletteIndex;
	CRGB* leds;
public:
	void initEffect(CRGB rawLEDBuffer[]);
	void updateEffect();
	unsigned long sumLeds();
};

#endif /* EFFECTS_LUMUEFFECT_H_ */
