#ifndef MATRIX_H_
#define MATRIX_H_

#include "FastLED.h"
#include "BoardSettings.h"

#define NUMBER_OF_ROWS 4
#define NUMBER_OF_COLUMNS 10
#define MIN_ROW (0)
#define MAX_ROW (NUMBER_OF_ROWS - 1)
#define MIN_COL (0)
#define MAX_COL (NUMBER_OF_COLUMNS - 1)

class Matrix {

	/**
	 * Pointer to the raw LED Buffer
	 */
	CRGB* rawLEDBuffer;
public:
	/**
		 * Buffer for effects
		 * Row #0 contains the center dot - should be avaraged before setting as the color of the center LED
		 * Row #1-#3 contains the center, middle, outer star leds
		 */
		CRGB ledMatrix[NUMBER_OF_ROWS][NUMBER_OF_COLUMNS];
	Matrix(CRGB* rawLEDBuffer);
	virtual ~Matrix();
	CRGB getCentralDotAvarageColor();
	void showDots();
	void getDot(int row, int col, CRGB& color);
	void blur();
	void setAllLEDs(CRGB& color);
	void clearCentralDot();
};

#endif /* MATRIX_H_ */
