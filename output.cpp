#include "Arduino.h"
#include <LiquidCrystal.h>
#include "output.h"

void Output::begin(boolean serial, LiquidCrystal* lcd) {
  this->serialEnabled = serial;
  this->lcd = lcd;
  if (lcd != NULL) {
    this->lcd->setCursor(0,0);
  }
}

void Output::output(String message) {
  if (Serial && this->serialEnabled) {
    Serial.print(message);
  }
  if (this->lcd != NULL) {
    this->lcd->print(message);
  }
}

void Output::outputln(String message) {
  if (Serial && this->serialEnabled) {
    Serial.println(message);
  }
  if (this->lcd != NULL) {
    this->lcd->print(message);
    this->lcd->setCursor(1,0);
  }
}


void Output::output(int value) {
  String message = String(value);
  if (Serial && this->serialEnabled) {
    Serial.print(message);
  }
  if (this->lcd != NULL) {
    this->lcd->print(message);
  }
}
