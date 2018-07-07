#include "debug.h"
#include "Matrix.h"
#include "BoardSettings.h"

void dbgPrint(char* message) {
#ifdef DEBUG
	Serial.print(message);
#endif
}

void dbgPrintln(char* message) {
#ifdef DEBUG
	Serial.println(message);
#endif
}

template<class T> inline Print &operator <<(Print &obj, T arg){
#ifdef DEBUG
	obj.print(arg);
#endif
	return obj;
}

void printColor(CRGB color, char* label) {
#ifdef DEBUG
	Serial.print(label);
	Serial.print(":");
	Serial.print(color.r);
	Serial.print(",");
	Serial.print(color.g);
	Serial.print(",");
	Serial.print(color.b);
	Serial.println();
#endif
}

void dumpDots(Matrix matrix) {
	Serial.println("\r\n\r\n--- Matrix dump: ---");
	for (int row = 0; row < NUMBER_OF_ROWS; row++) {
		Serial.print("Row");
		Serial.print(row);
		Serial.print(": ");
		for (int column = 0; column < NUMBER_OF_COLUMNS; column++) {
			Serial.print(matrix.ledMatrix[row][column].r);
			Serial.print("/");
			Serial.print(matrix.ledMatrix[row][column].g);
			Serial.print("/");
			Serial.print(matrix.ledMatrix[row][column].b);
			Serial.print(", ");
		}
		Serial.println();

	}
	Serial.println("--- End of Dump ---");
}

void dumpRaw(CRGB rawLEDBuffer[]) {
	Serial.println("\r\n Raw:");
	for (int i = 0; i < NUMBER_OF_LEDS; i++) {
		Serial.print(i);
		Serial.print(": [");
		Serial.print(rawLEDBuffer[i].r);
		Serial.print("/");
		Serial.print(rawLEDBuffer[i].g);
		Serial.print("/");
		Serial.print(rawLEDBuffer[i].b);
		Serial.print("], ");
		Serial.println();
	}
	Serial.println();
}
