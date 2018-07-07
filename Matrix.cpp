/*
 * Matrix.cpp
 *
 *  Created on: 2017. dec. 10.
 *      Author: Acer
 */

#include "Matrix.h"

Matrix::Matrix(CRGB rawLEDBuffer[NUMBER_OF_LEDS]) {
	this->rawLEDBuffer = rawLEDBuffer;
}

Matrix::~Matrix() {
	// TODO Auto-generated destructor stub
}

CRGB Matrix::getCentralDotAvarageColor() {

	CRGB result;
	int r = 0, g = 0, b = 0;
	for (int column = 0; column < NUMBER_OF_COLUMNS; column++) {
		r += ledMatrix[0][column].red;
		g += ledMatrix[0][column].green;
		b += ledMatrix[0][column].blue;
	}
	result.red = r / NUMBER_OF_COLUMNS;
	result.green = g / NUMBER_OF_COLUMNS;
	result.blue = b / NUMBER_OF_COLUMNS;
	return result;
}

void Matrix::showDots() {
	rawLEDBuffer[0] = getCentralDotAvarageColor();

	//memcpy(&rawLEDBuffer[1], &ledMatrix[1][0], 90);
	int index = 1;
	for (int row = 1; row < NUMBER_OF_ROWS; row++) {
		for (int column = 0; column < NUMBER_OF_COLUMNS; column++) {
			this->rawLEDBuffer[index++] = ledMatrix[row][column];
		}
	}

	FastLED.show();
}

void Matrix::getDot(int row, int col, CRGB& color) {

	col = col % NUMBER_OF_COLUMNS;

	if (row < MIN_ROW || row > MAX_ROW) {
		color = CRGB::Black;
	} else {
		color = ledMatrix[row][col];
	}
}

void Matrix::blur() {
	for (int row = 1; row < NUMBER_OF_ROWS; row++) {
		for (int column = 0; column < NUMBER_OF_COLUMNS; column++) {
			CRGB color;
			CRGB color2;
			getDot(row, column + 1, color);
			color /= 4;

			getDot(row - 1, column, color2);
			color2 /= 4;

			this->ledMatrix[row][column] = color + color2;
		}
	}
}

void Matrix::setAllLEDs(CRGB& color) {
	for (int row = 0; row < NUMBER_OF_ROWS; row++) {
		for (int column = 0; column < NUMBER_OF_COLUMNS; column++) {
			this->ledMatrix[row][column] = color;
		}
	}
}

void Matrix::clearCentralDot() {
	for (int i = 0; i < NUMBER_OF_COLUMNS; i++) {
		ledMatrix[0][i] = CRGB(random(255), random(255), random(255));
	}
}
