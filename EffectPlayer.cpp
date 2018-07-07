/*
 * EffectPlayer.cpp
 *
 *  Created on: 2017. dec. 13.
 *      Author: Acer
 */

#include "EffectPlayer.h"
#include "debug.h"
#include <HardwareSerial.h>

EffectPlayer::EffectPlayer(EffectPlayer&& argument) {
}

EffectPlayer::EffectPlayer() {
}

EffectPlayer::EffectPlayer(CRGB* ledBuffer) {
	this->currentEffect = EFFECT_START;
	this->ledBuffer = ledBuffer;
	this->initEffect();
}

void EffectPlayer::initEffect() {

	switch (currentEffect) {

	case EFFECT_CANDLE:
		dbgPrintln("CANDLE");
		effects.candleEffect.initEffect(ledBuffer);
		break;
	case EFFECT_LUMU:
		dbgPrintln("LUMU");
		effects.lumuEffect.initEffect(ledBuffer);
		break;
	case EFFECT_TRANSITION_DOT_UP_HSV:
		dbgPrintln("TDot");
		effects.transitionEffect.initEffect(ledBuffer, DOT, STYLE_CONTINOUS_UP,
				COLOR_HSV);
		break;
	case EFFECT_TRANSITION_RADII_UP_HSV:
		dbgPrintln("TRadii");
		effects.transitionEffect.initEffect(ledBuffer, RADII,
				STYLE_CONTINOUS_UP, COLOR_HSV);
		break;
	case EFFECT_TRANSITION_STAR_UP_HSV:
		dbgPrintln("TStar");
		effects.transitionEffect.initEffect(ledBuffer, STAR, STYLE_CONTINOUS_UP,
				COLOR_HSV);
		break;

	case EFFECT_TRANSITION_DOT_RANDOM_HSV:
		dbgPrintln("TDot");
		effects.transitionEffect.initEffect(ledBuffer, DOT, STYLE_RANDOM,
				COLOR_HSV);
		break;
	case EFFECT_TRANSITION_RADII_RANDOM_HSV:
		dbgPrintln("TRadii");
		effects.transitionEffect.initEffect(ledBuffer, RADII, STYLE_RANDOM,
				COLOR_HSV);
		break;
	case EFFECT_TRANSITION_STAR_RANDOM_HSV:
		dbgPrintln("TStar");
		effects.transitionEffect.initEffect(ledBuffer, STAR, STYLE_RANDOM,
				COLOR_HSV);
		break;

	case EFFECT_FIRE:
		//dbgPrintln("Fire");
		effects.fireEffect.initEffect(ledBuffer);
		break;
	}
}

void EffectPlayer::nextEffect() {
	currentEffect++;
	if (currentEffect > MAX_EFFECT) {
		currentEffect = MIN_EFFECT;
	}
	initEffect();
}

void EffectPlayer::prevEffect() {
	currentEffect--;
	if (currentEffect < MIN_EFFECT) {
		currentEffect = MAX_EFFECT;
	}
	initEffect();
}

void EffectPlayer::updateEffect() {
	switch (currentEffect) {

	case EFFECT_TRANSITION_DOT_RANDOM_HSV:
	case EFFECT_TRANSITION_DOT_UP_HSV:
	case EFFECT_TRANSITION_RADII_RANDOM_HSV:
	case EFFECT_TRANSITION_RADII_UP_HSV:
	case EFFECT_TRANSITION_STAR_RANDOM_HSV:
	case EFFECT_TRANSITION_STAR_UP_HSV:

		effects.transitionEffect.updateEffect();
		break;

	case EFFECT_FIRE:
		effects.fireEffect.updateEffect();
		break;
	case EFFECT_LUMU:
		effects.lumuEffect.updateEffect();
		break;
	case EFFECT_CANDLE:
		effects.candleEffect.updateEffect();
		break;
	}
}
