
#ifndef KEYPAD_H
#define	KEYPAD_H
#include "Arduino.h"
#include <HardwareSerial.h>

#define KEYPAD_PINS {3,4,5,6,7,8,9}
#define KEYPAD_PIN_COUNT 7

class Keypad {
public:
  int pins[KEYPAD_PIN_COUNT];
  int pinCount;
Keypad();
void debugMap();
void begin();
};

#endif
