#include "ChristmasStar.h"
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <avr/power.h>
#include <SparkFun_APDS9960.h>

#include "EffectPlayer.h"
#include <LowPower.h>
#include <avr/pgmspace.h>

#define MAX_BRIGHTNESS (20)
#define INITIAL_BRIGHTNESS (10)
#define MIN_BRIGHTNESS (5)
#define NO_GESTURE_TIME (1000)

struct LIGHTDATA {
	uint16_t red;
	uint16_t green;
	uint16_t blue;
	uint16_t ambient;
};

uint8_t brightness;

uint16_t frame = 0;

EffectPlayer player = EffectPlayer(ledBuffer);

SparkFun_APDS9960 apds = SparkFun_APDS9960();

bool interruptReceived = 0;

CRGB saveBuffer[NUMBER_OF_LEDS];

void drawRadii(uint8_t startIndex, bool drawCenter, CRGB color) {
	if (startIndex <= 10) {
		if (drawCenter) {
			ledBuffer[0] = color;
		}
		for (uint8_t i = startIndex; i <= 30; i += 10) {
			ledBuffer[i] = color;
		}
	}
}

enum ARROW {
	NONE, UP, DOWN, LEFT, RIGHT
};

ARROW arrowOverlay;
unsigned long displayArrowUntil;

void drawCenterArrow(uint8_t startIndex1, uint8_t startIndex2,
		uint8_t startIndex3, CRGB color) {
	drawRadii(startIndex1, true, color);
	drawRadii(startIndex2, false, color);
	drawRadii(startIndex3, false, color);
}

void drawArrow(ARROW arrow, CRGB color) {
	switch (arrow) {
	case UP:
		drawCenterArrow(4, 6, 8, color);
		break;
	case DOWN:
		drawCenterArrow(1, 2, 10, color);
		break;
	case LEFT:
		drawCenterArrow(2, 3, 4, color);
		break;
	case RIGHT:
		drawCenterArrow(8, 9, 10, color);
		break;
	case NONE:
		break;
	}
}

void resetAPDSInterrupt() {
	interruptReceived = false;
}

void receiveAPDSInterrupt() {
	interruptReceived = true;
}

void updateBrightness() {
	FastLED.setBrightness(brightness);
}

void increaseBrightness() {
	/*	if (brightness < MAX_BRIGHTNESS) {
	 brightness++;
	 }
	 updateBrightness();*/
}

volatile int proximitInterruptValue;

void sleepInterrupt() {
	noInterrupts();
	detachInterrupt(0);

	proximitInterruptValue++;

	interrupts();
}

bool enableProximitySensorInt(uint8_t pgain, uint8_t ldrive,
		uint8_t nearTreshold, uint8_t farTreshold) {
	/* Set default gain, LED, interrupts, enable power, and enable sensor */
	if (!apds.setProximityGain(pgain)) {
		return false;
	}
	if (!apds.setLEDDrive(ldrive)) {
		return false;
	}

	if (!apds.setProximityIntHighThreshold(farTreshold)) {
		return false;
	}

	if (!apds.setProximityIntLowThreshold(nearTreshold)) {
		return false;
	}

	if (!apds.setProximityIntEnable(1)) {
		return false;
	}

	if (!apds.enablePower()) {
		return false;
	}
	if (!apds.setMode(PROXIMITY, 1)) {
		return false;
	}

	return true;
}
#pragma GCC push_options
#pragma GCC optimize ("O0")
void emptyGestureQueue() {

	while (apds.isGestureAvailable()) {
		int gesture = apds.readGesture();

	}

}
#pragma GCC pop_options

void enterSleepMode() {
	// disable APA102
	pinMode(APA102_ENABLE_PIN, INPUT);
	digitalWrite(APA102_ENABLE_PIN, HIGH);

	//Serial.println("Sleep...");

	detachInterrupt(0);

	apds.init();
	//emptyGestureQueue();
	enableProximitySensorInt(0, 3, 0, 10);
	apds.clearProximityInt();
	attachInterrupt(0, sleepInterrupt, LOW);

	LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_ON);
	detachInterrupt(0);
	setup();
}

void displayArrow(ARROW arrow, unsigned long displayTimeMS) {
	arrowOverlay = arrow;
	displayArrowUntil = millis() + displayTimeMS;
}
unsigned long noGestureUntilMs = 0;

void printGesture(int gesture) {
	switch (gesture) {
				case DIR_UP:
					Serial.println(F("UP"));
					break;
				case DIR_DOWN:
					Serial.println(F("DOWN"));
					break;
				case DIR_LEFT:
					Serial.println(F("LEFT"));
					break;
				case DIR_RIGHT:
					Serial.println(F("RIGHT"));
					break;
				case DIR_NEAR:
					Serial.println(F("NEAR"));
					break;
				case DIR_FAR:
					Serial.println(F("FAR"));
					break;
				default:
					Serial.println(F("NONE"));
					break;
				}
}

bool handleGesture() {
	//Serial.println("Handle Gesture");
	if (apds.isGestureAvailable()) {

		//Serial.println("Gesture available...");

		int gesture = apds.readGesture();
		//printGesture(gesture);
		bool processGesture = noGestureUntilMs < millis();
		//Serial.print("Process gesture: ");
		//Serial.println(processGesture);

		if (processGesture) {
			switch (gesture) {
			case DIR_UP:
				displayArrow(UP, 1000);
//				increaseBrightness();
				break;
			case DIR_DOWN:
				enterSleepMode();
				break;
			case DIR_LEFT:
				noGestureUntilMs = millis() + NO_GESTURE_TIME;
				displayArrow(RIGHT, 1000);
				player.prevEffect();
				break;
			case DIR_RIGHT:
				noGestureUntilMs = millis() + NO_GESTURE_TIME;
				displayArrow(LEFT, 1000);
				player.nextEffect();
				break;
			case DIR_NEAR:
				break;
			case DIR_FAR:
				break;
			default:
				break;
			}
		}
		return true;
	} else {
		//Serial.println("No gesture available");
	}
	return false;
}
void attachAPDSInterrupt() {
	attachInterrupt(APDS_INTERRUPT, receiveAPDSInterrupt, FALLING);
}

void detachAPDSInterrupt() {
	detachInterrupt(APDS_INTERRUPT);
}

void setup() {

	brightness = INITIAL_BRIGHTNESS;

	pinMode(APA102_ENABLE_PIN, OUTPUT);
	digitalWrite(APA102_ENABLE_PIN, LOW);

	FastLED.addLeds<APA102, BGR>(ledBuffer, NUMBER_OF_LEDS);
	//FastLED.setMaxPowerInVoltsAndMilliamps(5, 100);
	updateBrightness();
	//setDots(CRGB::Black);
	//Serial.begin(115200);
	if (apds.init()) {
		//Serial.println(F("APDS OK"));
		//emptyGestureQueue();
		apds.enableGestureSensor(true);
		apds.setLEDDrive(3);
		apds.clearProximityInt();
		attachAPDSInterrupt();
		noGestureUntilMs = millis() + NO_GESTURE_TIME;
	}

	//Serial.println(F("Setup finished..."));
}

/**
 * Check if Light reading is available
 */
void handleLight() {
	LIGHTDATA light;

	// Read the light levels (ambient, red, green, blue)
	if (!apds.readAmbientLight(light.ambient) || !apds.readRedLight(light.red)
			|| !apds.readGreenLight(light.green)
			|| !apds.readBlueLight(light.blue)) {
		//	Serial.println(F("Error reading light values"));
	} else {
		/*		Serial.print(F("Ambient: "));
		 Serial.print(light.ambient);
		 Serial.print(F(" Red: "));
		 Serial.print(light.red);
		 Serial.print(F(" Green: "));
		 Serial.print(light.green);
		 Serial.print(F(" Blue: "));
		 Serial.println(light.blue);*/
	}
}

void handleAPDS() {
	if (interruptReceived) {
		detachAPDSInterrupt();
//		Serial.println(F("APDS INT"));
//		handleLight();
		handleGesture();

		resetAPDSInterrupt();
		attachAPDSInterrupt();
	}
}

void darken(CRGB color) {
	for (int i = 0; i < NUMBER_OF_LEDS; i++) {
		ledBuffer[i] -= color;
	}
}

CRGB darkColor(50, 50, 50);

void drawOverlay() {
	if (arrowOverlay != NONE && millis() < displayArrowUntil) {
		//memcpy(saveBuffer, ledBuffer, sizeof(saveBuffer));
		darken(darkColor);
		drawArrow(arrowOverlay, CRGB::White);
		FastLED.show();
		//memcpy(ledBuffer, saveBuffer, sizeof(ledBuffer));
	} else {
		FastLED.show();
	}
}

void loop() {
	player.updateEffect();
	drawOverlay();
	handleAPDS();
}

