#include <Wire.h>
#include <SPI.h>
#include <HardwareSerial.h>
#include "ioexpander.h"
#include "mixer.h"
#include "keypad.h"

  int readvalue; 
  Mixer aMixer;
  Keypad aKeypad;
  
  #define LED 108

void setup(){ 
  
  aMixer.begin();
  aMixer.setReadModeSPI(10);
  Serial.begin(115200); 
  if (!Serial) {
    while (!Serial) {
      delay(1);
    }
  }
  aKeypad.begin();
}


void loop() {
  static char readKey;
  float Grams;
  static int recipeIndex = 0;
  Grams = aMixer.getGrams();
  Serial.print(Grams);
  Serial.println(" grams");
  for (int i = 0; i<=9; i++) {
    if (readKey != -1) {    // we didn't just read a key
      readKey = aKeypad.keyScan();
      switch (readKey) {
        case 'Z':
          Serial.println("Resetting...");
          aMixer.resetZero();
          break;
        case 'C':
          if (++recipeIndex > MAX_RECIPES) {
            recipeIndex = MAX_RECIPES;
          }
          Serial.print("Recipe ");
          Serial.print(recipeIndex);
          Serial.println(" selected (Jump to run)");
          break;
        case 'c':
          if (--recipeIndex < 0) {
            recipeIndex = 0;
          }
          Serial.print("Recipe ");
          Serial.print(recipeIndex);
          Serial.println(" selected (Jump to run)");
          break;
        case 'J':
          Serial.print("Running Recipe ");
          Serial.print(recipeIndex);
          Serial.println("...");
          aMixer.runRecipe(recipeIndex);
          break;
      }
    }
    if (readKey != 0) {
      readKey = -1; // if we processed a key, ignore keys for half a second.
    }
    delay(50); //50ms * 10 = 1/2 second visual updates, but 50ms keyscans
  }
  readKey = 0;
}
