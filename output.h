#ifndef OUTPUT_H
#define	OUTPUT_H
#include "Arduino.h"
#include <LiquidCrystal.h>

class Output {
  boolean serialEnabled;
  LiquidCrystal *lcd;
  int lcdWidth;
  int cursorX, cursorY;
public:
  void begin(boolean, LiquidCrystal*, int);
  void output(String);
  void outputln(String);
  void output(int);
  void setCursor(int,int);
};
#endif
