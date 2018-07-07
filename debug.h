#ifndef DEBUG_H_
#define DEBUG_H_

#define DEBUG 0

#include <FastLED.h>
class Print;

#define dbg (Serial)

void dbgPrint(char* message);
void dbgPrintln(char* message);

void printColor(CRGB color, char* label);

template<class T> inline Print &operator <<(Print &obj, T arg);

void dumpRaw(CRGB rawLEDBuffer[]);

#endif /* DEBUG_H_ */
