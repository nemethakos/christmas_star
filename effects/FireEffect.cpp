#include "FireEffect.h"
// Fire2012 by Mark Kriegsman, July 2012
// as part of "Five Elements" shown here: http://youtu.be/knWiGsmgycY
////
// This basic one-dimensional 'fire' simulation works roughly as follows:
// There's a underlying array of 'heat' cells, that model the temperature
// at each point along the line.  Every cycle through the simulation,
// four steps are performed:
//  1) All cells cool down a little bit, losing heat to the air
//  2) The heat from each cell drifts 'up' and diffuses a little
//  3) Sometimes randomly new 'sparks' of heat are added at the bottom
//  4) The heat from each cell is rendered as a color into the leds array
//     The heat-to-color mapping uses a black-body radiation approximation.
//
// Temperature is in arbitrary units from 0 (cold black) to 255 (white hot).
//
// This simulation scales it self a bit depending on NUM_LEDS; it should look
// "OK" on anywhere from 20 to 100 LEDs without too much tweaking.
//
// I recommend running this simulation at anywhere from 30-100 frames per second,
// meaning an interframe delay of about 10-35 milliseconds.
//
// Looks best on a high-density LED setup (60+ pixels/meter).
//
//
// There are two main parameters you can play with to control the look and
// feel of your fire: COOLING (used in step 1 above), and SPARKING (used
// in step 3 above).
//
// COOLING: How much does the air cool as it rises?
// Less cooling = taller flames.  More cooling = shorter flames.
// Default 55, suggested range 20-100
#define COOLING  200

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 150
#define BRIGHTNESS  200

#define CELL_NUM (HEAT_ROW_NUM * HEAT_COL_NUM)

FireEffect::FireEffect() {
}

void FireEffect::initEffect(CRGB rawLEDBuffer[]) {
	frame = 0;
	ledBuffer = rawLEDBuffer;
	// This first palette is the basic 'black body radiation' colors,
	// which run from black to red to bright yellow to white.
	gPal = HeatColors_p;

	// These are other ways to set up the color palette for the 'fire'.
	// First, a gradient from black to red to yellow to white -- similar to HeatColors_p
	//   gPal = CRGBPalette16( CRGB::Black, CRGB::Red, CRGB::Yellow, CRGB::White);

	// Second, this palette is like the heat colors, but blue/aqua instead of red/yellow
	//   gPal = CRGBPalette16( CRGB::Black, CRGB::Blue, CRGB::Aqua,  CRGB::White);

	// Third, here's a simpler, three-step gradient, from black to red to white
	//   gPal = CRGBPalette16( CRGB::Black, CRGB::Red, CRGB::White);
	for (int row = 0; row < HEAT_ROW_NUM; row++) {
		for (int col = 0; col < HEAT_COL_NUM; col++) {
			heat[row][col] = 0;
		}
	}
}

// Fire2012 with programmable Color Palette
//
// This code is the same fire simulation as the original "Fire2012",
// but each heat cell's temperature is translated to color through a FastLED
// programmable color palette, instead of through the "HeatColor(...)" function.
//
// Four different static color palettes are provided here, plus one dynamic one.
//
// The three static ones are:
//   1. the FastLED built-in HeatColors_p -- this is the default, and it looks
//      pretty much exactly like the original Fire2012.
//
//  To use any of the other palettes below, just "uncomment" the corresponding code.
//
//   2. a gradient from black to red to yellow to white, which is
//      visually similar to the HeatColors_p, and helps to illustrate
//      what the 'heat colors' palette is actually doing,
//   3. a similar gradient, but in blue colors rather than red ones,
//      i.e. from black to blue to aqua to white, which results in
//      an "icy blue" fire effect,
//   4. a simplified three-step gradient, from black to red to white, just to show
//      that these gradients need not have four components; two or
//      three are possible, too, even if they don't look quite as nice for fire.
//
// The dynamic palette shows how you can change the basic 'hue' of the
// color palette every time through the loop, producing "rainbow fire".

void FireEffect::updateEffect() {

	// Add entropy to random number generator; we use a lot of it.
	random16_add_entropy(random());

	// Fourth, the most sophisticated: this one sets up a new palette every
	// time through the loop, based on a hue that changes every time.
	// The palette is a gradient from black, to a dark color based on the hue,
	// to a light color based on the hue, to white.
	//
	//   static uint8_t hue = 0;
	//   hue++;
	//   CRGB darkcolor  = CHSV(hue,255,192); // pure hue, three-quarters brightness
	//   CRGB lightcolor = CHSV(hue,128,255); // half 'whitened', full brightness
	//   gPal = CRGBPalette16( CRGB::Black, darkcolor, lightcolor, CRGB::White);



	if (frame == 100) {
		Fire2012WithPalette(); // run simulation frame, using palette colors
		frame = 0;
	}
	frame++;
}

int FireEffect::getHeat(int row, int col) {
	if (row < 0 || row > HEAT_ROW_NUM - 1 || col < 0 || col > HEAT_COL_NUM) {
		return 0;
	} else {
		return heat[row][col];
	}
}

void FireEffect::Fire2012WithPalette() {

	// Step 1.  Cool down every cell a little
	for (int row = 0; row < HEAT_ROW_NUM; row++) {
		for (int col = 0; col < HEAT_COL_NUM; col++) {
			heat[row][col] = qsub8(getHeat(row, col),
					random8(0, ((COOLING * 10) / CELL_NUM) + 2));
		}
	}

	// Step 2.  Heat from each cell drifts 'up' and diffuses a little
	for (int row = HEAT_ROW_NUM - 1; row >= 1; row--) {
		for (int col = 0; col < HEAT_COL_NUM; col++) {
			heat[row][col] = (getHeat(row - 1, col) + getHeat(row - 2, col)
					+ getHeat(row - 2, col)) / 3;
		}
	}

	/*	for (int k = NUM_LEDS - 1; k >= 2; k--) {
	 heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
	 }*/

	// Step 3.  Randomly ignite new 'sparks' of heat near the bottom
	if (random8() < SPARKING) {
		//int col = random8(HEAT_COL_NUM);
		int row = 0;
		for (int col = 0; col < HEAT_COL_NUM; col++) {
			heat[0][col] = qadd8(heat[row][col], random8(10, 50));
		}
	}

// map row #0

	uint16_t sum = 0;
	for (int col = 0; col < HEAT_COL_NUM; col++) {
		sum += heat[0][col];
	}
	sum /= HEAT_COL_NUM;
	ledBuffer[0] = map(sum);

	for (int row = 1; row < HEAT_ROW_NUM; row++) {
		for (int col = 0; col < HEAT_COL_NUM; col++) {
			ledBuffer[mapPixel(row, col)] = map(heat[row - 1][col]);
		}
	}
}

int FireEffect::mapPixel(int row, int col) {
	switch (row) {
	case 0:
		return 0;
		break;
	case 1:
		return 1 + col;
		break;
	case 2:
		return 11 + col;
		break;
	case 3:
		return 21 + col;
		break;
	}
	return 0;
}

CRGB FireEffect::map(uint8_t hotness) {
// Scale the heat value from 0-255 down to 0-240
// for best results with color palettes.
	byte colorindex = scale8(hotness, 240);
	CRGB color = ColorFromPalette(gPal, colorindex);
	return color;
}
