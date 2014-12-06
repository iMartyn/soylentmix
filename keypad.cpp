#include "keypad.h"
#include <HardwareSerial.h>

Keypad::Keypad() {
}

void Keypad::begin() {
  int arr[] = KEYPAD_PINS;
  this->pinCount = KEYPAD_PIN_COUNT;
  for (int i = 0; i < this->pinCount; i++) {
    this->pins[i] = arr[i];
  }
  for (int i = 0; i < this->pinCount; i++) {
    digitalWrite(this->pins[i], HIGH);
    pinMode(this->pins[i], INPUT_PULLUP);
  }
}

void Keypad::debugMap() {
//  while (true) {
//  this->pinCount = KEYPAD_PIN_COUNT;
  int i = 0;
    for (i = 0; i < pinCount; i++) {
      pinMode(this->pins[i], OUTPUT);
      digitalWrite(this->pins[i], LOW);
      for (int j = i+1; j < this->pinCount; j++) {
        if (digitalRead(this->pins[j]) == LOW) {
          Serial.print(i);
          Serial.print(" ");
          Serial.println(j);
        }
      }
      digitalWrite(this->pins[i], HIGH);
      pinMode(this->pins[i], INPUT_PULLUP);
      delay(5);
      Serial.println("Scanned.");
    }
    return;
  //}
}
