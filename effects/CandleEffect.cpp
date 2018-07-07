#include "CandleEffect.h"

void CandleEffect::updateLedsForCandles() {
	for (int i = 0; i < NUMBER_OF_LEDS; i++) {

	    byte candleIndex = candleMap[i];
	    int index  = candle[candleIndex].level >> 8;

	    byte colorindex = scale8( index, 250);
	    /* Serial.print(index);
	     Serial.print("->");
	     Serial.println(colorindex);*/
	    CRGB color = ColorFromPalette( heatMap, colorindex);
	    leds[i] = color;
	    // printCRGB(&color);
	  }
}

// update single candle, set the result color in led
void CandleEffect::updateSingleCandle(int index) {

	CANDLE_DATA *c = &candle[index];
	// randomize candle if needed
	if (c->phase > c->delayEndPhase) {
		randomizeCandle(index);
	} else
	// reset phase counter on phase changes
	if ((c->phase == c->attackEndPhase + 1)
			|| (c->phase == c->sustainEndPhase + 1)
			|| (c->phase == c->releaseEndPhase + 1)) {
		c->phaseCounter = 0;
	}

	// attack phase
	if (c->phase <= c->attackEndPhase) {
		if (c->phaseCounter == 0) {
			// set initial level
			c->level = c->attackLevel << 8;
		} else {
			// increase level
			c->level += c->attackInc;
		}
	} else if (c->phase <= c->sustainEndPhase) {
		if (c->phaseCounter == 0) {
			c->level = c->sustainLevel << 8;
		}
	} else if (c->phase <= c->releaseEndPhase) {
		if (c->phaseCounter > 0) {
			c->level -= c->releaseDec;
		}
	}
	c->phase++;
	c->phaseCounter++;

}
void CandleEffect::updateEffect() {

	for (int i = 0; i < NUMBER_OF_CANDLES; i++) {
		updateSingleCandle(i);
	}
	updateLedsForCandles();
}

// randomize candle
void CandleEffect::randomizeCandle(int index) {
	CANDLE_DATA *c = &candle[index];
	if (c->phase == 0) {
		// start level
		c->attackLevel = 0;
		c->level = 0;
	} else {
		c->attackLevel = c->delayLevel;
		c->level = c->attackLevel << 8;
	}
	// high level
	c->sustainLevel = random(MIN_SUSTAIN_LEVEL, MAX_SUSTAIN_LEVEL);
	// end level
	c->delayLevel = random(MIN_DELAY_LEVEL, MAX_DELAY_LEVEL);
	// number of phases for attack
	c->attackEndPhase = random(MIN_ATTACK_PHASE_NUM, MAX_ATTACK_PHASE_NUM);
	c->sustainEndPhase = random(c->attackEndPhase + MIN_SUSTAIN_PHASE_NUM,
			c->attackEndPhase + MAX_SUSTAIN_PHASE_NUM);
	c->releaseEndPhase = random(c->sustainEndPhase + MIN_RELEASE_PHASE_NUM,
			c->sustainEndPhase + MAX_RELEASE_PHASE_NUM);

	c->delayEndPhase = random(c->releaseEndPhase + MIN_DELAY_PHASE_NUM,
			c->releaseEndPhase + MAX_DELAY_PHASE_NUM);
	// current phase
	c->phase = 0;
	c->phaseCounter = 0;
	c->attackInc = ((c->sustainLevel - c->attackLevel) << 8)
			/ c->attackEndPhase;

	unsigned int releaseLength = c->releaseEndPhase - c->sustainEndPhase;
	c->releaseDec = ((c->sustainLevel - c->delayLevel) << 8) / releaseLength;
}

void CandleEffect::initEffect(CRGB* rawLEDBuffer) {
	//Serial.println("CANDLE INIT START");
	leds = rawLEDBuffer;
	// init heat map

	heatMap = CRGBPalette16(CRGB::Black, CRGB::Red, CRGB::Yellow);
	/*
	 for (int i = 0; i < 0xff; i++) {
	 CHSV colorHSV = CHSV(i >> 2, 255, 0x80 + i >> 1);
	 CRGB colorRGB;
	 hsv2rgb_rainbow(colorHSV, colorRGB);
	 u.candleEffectData.heatMap[i * 3] = colorRGB.r;
	 u.candleEffectData.heatMap[i * 3 + 1] = colorRGB.g;
	 u.candleEffectData.heatMap[i * 3 + 2] = colorRGB.b;
	 }
	 */

	// init candles
	for (int i = 0; i < NUMBER_OF_CANDLES; i++) {
		// this indicates the initial state
		candle[i].phase = 0;
		randomizeCandle(i);
	}
	for (int i = 0; i < NUMBER_OF_LEDS; i++) {
	    candleMap[i] = random(0, NUMBER_OF_CANDLES - 1);
	  }
}
