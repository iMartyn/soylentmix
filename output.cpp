#include "Arduino.h"
#include <LiquidCrystal.h>
#include "output.h"

void Output::begin(boolean serial, LiquidCrystal* lcd, int lcdWidth) {
  this->serialEnabled = serial;
  this->lcd = lcd;
  if (lcd != NULL) {
    this->lcd->setCursor(0,0);
    this->cursorX = 0;
    this->cursorY = 0;
  }
  this->lcdWidth = lcdWidth;
}

void Output::output(String message) {
  if (Serial && this->serialEnabled) {
    Serial.print(message);
  }
  if (this->lcd != NULL) {
    this->lcd->print(message);
    this->cursorX += message.length();
  }
}

void Output::outputln(String message) {
  if (Serial && this->serialEnabled) {
    Serial.println(message);
  }
  if (this->lcd != NULL) {
    int currentX = this->cursorX;
    int currentY = this->cursorY;
    this->lcd->setCursor(this->cursorX,this->cursorY);
    for (int i=this->cursorX+1; i<this->lcdWidth; i++) {
      this->setCursor(i,currentY);
      this->lcd->print(" ");
    }
    this->lcd->setCursor(currentX,currentY);
    this->lcd->print(message);
    this->lcd->setCursor(0,currentY+1);
  }
}


void Output::output(int value) {
  String message = String(value);
  if (Serial && this->serialEnabled) {
    Serial.print(message);
  }
  if (this->lcd != NULL) {
    this->lcd->print(message);
    this->cursorX += message.length();
  }
}

void Output::setCursor(int x, int y) {
  this->lcd->setCursor(x,y);
  this->cursorX = x;
  this->cursorY = y;
}
