#include <EEPROM.h>
#include <avr/eeprom.h>
#include <LiquidCrystal.h>
#include <Wire.h>
#include <SPI.h>
#include <HardwareSerial.h>
#include <LiquidCrystal.h>
#include "ioexpander.h"
#include "mixer.h"
#include "keypad.h"
#include "output.h"

  int readvalue; 
  Mixer aMixer;
  Keypad aKeypad;
  LiquidCrystal lcd(LCD_PINS);
  Output output;
  #define LED 108
  
  struct settings_t
{
  char signature[8];
  int recipes[MAX_RECIPES][MAX_HOPPERS];
} settings;

void setup(){ 
  
  aMixer.begin();
  aMixer.setReadModeSPI(10);
  Serial.begin(115200); 
  if (!Serial) {
    while (!Serial) {
      delay(1);
    }
  }
  readRecipes();
  aKeypad.begin();
  output.begin(true,&lcd);
}

void readRecipes() {
  eeprom_read_block((void*)&settings, (void*)0, sizeof(settings));
  if (settings.signature != "Recipes") {
    writeRecipes();
  }
}

void writeRecipes() {
  memcpy(settings.signature,(void*)"Recipes",8);
  memcpy(settings.recipes,aMixer.recipes,sizeof(settings.recipes));
  eeprom_write_block((const void*)&settings, (void*)0, sizeof(settings));
}

void loop() {
  static char readKey;
  float Grams;
  static int recipeIndex = 0;
  Grams = aMixer.getGrams();
  output.output(Grams);
  output.outputln(" grams");
  for (int i = 0; i<=9; i++) {
    if (readKey != -1) {    // we didn't just read a key
      readKey = aKeypad.keyScan();
      switch (readKey) {
        case 'Z':
          output.outputln("Resetting...");
          aMixer.resetZero();
          break;
        case 'C':
          if (++recipeIndex > MAX_RECIPES) {
            recipeIndex = MAX_RECIPES;
          }
          output.output("Recipe ");
          output.output(recipeIndex);
          output.outputln(" selected (Jump to run)");
          break;
        case 'c':
          if (--recipeIndex < 0) {
            recipeIndex = 0;
          }
          output.output("Recipe ");
          output.output(recipeIndex);
          output.outputln(" selected (Jump to run)");
          break;
        case 'J':
          output.output("Running Recipe ");
          output.output(recipeIndex);
          output.outputln("...");
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
