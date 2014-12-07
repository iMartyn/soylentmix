
#ifndef KEYPAD_H
#define	KEYPAD_H
#include "Arduino.h"
#include <HardwareSerial.h>

#define KEYPAD_PINS {3,4,5,6,7,8,9}
#define KEYPAD_PIN_COUNT 7
#define KEYCODES { { 0, '1', 0, '7', 0, 'Z', '4' }, { 0, 0, '2', '8', 'v', '0', '5' }, { 0, 0, 0, 0, 'V', 0, '3' }, { 0, 0, 0, 0, 'C', 'J', '9' }, { 0, 0, 0, 0, 0, 'c', 'P' }, { 0, 0, 0, 0, 0, 0, '6' }, { 0, 0, 0, 0, 0, 0, 0, } }

class Keypad {
public:
  int pins[KEYPAD_PIN_COUNT];
  int pinCount;
  int keyMap[KEYPAD_PIN_COUNT][KEYPAD_PIN_COUNT];
Keypad();
void begin();
void debugMap();
char keyScan();
};

#endif
