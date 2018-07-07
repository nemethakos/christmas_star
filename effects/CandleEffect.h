/*
 * CandleEffect.h
 *
 *  Created on: 2017. dec. 22.
 *      Author: Acer
 */

#ifndef EFFECTS_CANDLEEFFECT_H_
#define EFFECTS_CANDLEEFFECT_H_

#include "../BoardSettings.h"
#include "inttypes.h"
#include "FastLED.h"

#define NUMBER_OF_CANDLES (31)

#define MIN_DELAY_LEVEL 0
#define MAX_DELAY_LEVEL 100

#define MIN_SUSTAIN_LEVEL (MAX_DELAY_LEVEL)
#define MAX_SUSTAIN_LEVEL 255

#define MIN_ATTACK_PHASE_NUM 100
#define MAX_ATTACK_PHASE_NUM 500
#define MIN_SUSTAIN_PHASE_NUM 100
#define MAX_SUSTAIN_PHASE_NUM 1000
#define MIN_RELEASE_PHASE_NUM 100
#define MAX_RELEASE_PHASE_NUM 500
#define MIN_DELAY_PHASE_NUM 100
#define MAX_DELAY_PHASE_NUM 500

struct CANDLE_DATA {
	// start level
	uint8_t attackLevel;
	// 8.8 increase of attack phase
	unsigned int attackInc;
	// high level
	uint8_t sustainLevel;
	// 8.8 decrease of release phase
	unsigned int releaseDec;
	// level of delay phase
	uint8_t delayLevel;
	// phase counter of end of attack
	unsigned int attackEndPhase;
	// phase counter of end of sustain phase
	unsigned int sustainEndPhase;
	// phase counter of end of release phase
	unsigned int releaseEndPhase;
	// phase counter of end of delay phase (phase counter restarts to 0)
	unsigned int delayEndPhase;
	// current phase
	unsigned int phase;
	// counter inside the phase, reseted in every phase change
	uint8_t phaseCounter;
	// level 8.8
	unsigned int level;
};

class CandleEffect {
	// data for each candle
	CANDLE_DATA candle[NUMBER_OF_CANDLES];
	CRGB* leds;
	CRGBPalette16 heatMap;
	byte candleMap[NUMBER_OF_LEDS];
public:
	void initEffect(CRGB rawLEDBuffer[]);
	void updateEffect();
	void updateSingleCandle(int index);
	void randomizeCandle(int index);
	void updateLedsForCandles();
};

#endif /* EFFECTS_CANDLEEFFECT_H_ */
