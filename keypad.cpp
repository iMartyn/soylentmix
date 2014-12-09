#include "keypad.h"
#include "ioexpander.h"
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
  int arr2[KEYPAD_PIN_COUNT][KEYPAD_PIN_COUNT] = KEYCODES;
  Serial.println("Initialising pins : ");
  for (int i = 0; i < this->pinCount; i++) {
    for (int j = 0; j < this->pinCount; j++) {
      Serial.print(i);
      Serial.print(",");
      Serial.print(j);
      Serial.print(" = ");
      this->keyMap[i][j] = arr2[i][j];
      Serial.println(this->keyMap[i][j]);
    }
  }
}

void Keypad::debugMap() {
  Serial.println("KeyMap : ");
  for (int i = 0; i < this->pinCount; i++) {
    for (int j = 0; j < this->pinCount; j++) {
      Serial.print(i);
      Serial.print(",");
      Serial.print(j);
      Serial.print(" = ");
      Serial.println(this->keyMap[i][j]);
    }
  }
  while (true) {
    for (int i = 0; i < pinCount; i++) {
      pinMode(this->pins[i], OUTPUT);
      digitalWrite(this->pins[i], LOW);
      for (int j = i+1; j < this->pinCount; j++) {
        if (digitalRead(this->pins[j]) == LOW) {
          Serial.print(i);
          Serial.print(" ");
          Serial.print(j);
          Serial.print(" (");
          Serial.print((char)this->keyMap[i][j]);
          Serial.println(")");
        }
      }
      digitalWrite(this->pins[i], HIGH);
      pinMode(this->pins[i], INPUT_PULLUP);
      delay(5);
    }
  }
}

char Keypad::keyScan() {
  for (int i = 0; i < this->pinCount; i++) {
    for (int j = i; j < this->pinCount; j++) {
      pinMode(this->pins[i], OUTPUT);
      digitalWrite(this->pins[i], LOW);
      for (int j = i+1; j < this->pinCount; j++) {
        if (digitalRead(this->pins[j]) == LOW) {
          digitalWrite(this->pins[i], HIGH);
          pinMode(this->pins[i], INPUT_PULLUP);
          return this->keyMap[i][j];
        }
      }
      digitalWrite(this->pins[i], HIGH);
      pinMode(this->pins[i], INPUT_PULLUP);
    }
  }
  return 0;
}
