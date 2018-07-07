/*
 * EffectPlayer.h
 *
 *  Created on: 2017. dec. 13.
 *      Author: Acer
 */

#ifndef EFFECTPLAYER_H_
#define EFFECTPLAYER_H_

#include <stdint.h>

#include "effects/BlinkEffect.h"
#include "effects/StarEffect.h"
#include "effects/TransitionEffect.h"
#include "EffectType.h"
#include "effects/FireEffect.h"
#include "effects/LUMUEffect.h"
#include "effects/CandleEffect.h"

union EFFECTDATA {
	EFFECTDATA() {};
	int dummy;
	//BlinkEffect blinkEffect;
	//StarEffect starEffect;
	TransitionEffect transitionEffect;
	FireEffect fireEffect;
	LUMUEffect lumuEffect;
	CandleEffect candleEffect;
};

class EffectPlayer {

public:
	EffectPlayer();
	EffectPlayer(CRGB* ledBuffer);
	EffectPlayer(EffectPlayer&&);

	void nextEffect();
	void prevEffect();
	void updateEffect();
	void initEffect();
	EFFECTDATA effects;
	int currentEffect;
	CRGB* ledBuffer;
};

#endif /* EFFECTPLAYER_H_ */
