#ifndef OUTPUT_H
#define	OUTPUT_H
#include "Arduino.h"
#include <LiquidCrystal.h>

class Output {
  boolean serialEnabled;
  LiquidCrystal *lcd;
public:
  void begin(boolean, LiquidCrystal*);
  void output(String);
  void outputln(String);
  void output(int);
};
#endif
