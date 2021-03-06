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
#include "SoftwareServo.h"

  int readvalue; 
  Mixer aMixer;
  Keypad aKeypad;
  LiquidCrystal lcd(LCD_PINS);
  Output output;
  #define LED 108
  SoftwareServo testServo;
  
  struct settings_t
{
  char signature[8];
  int recipes[MAX_RECIPES][MAX_HOPPERS];
} settings;

void setup(){ 
  
  aMixer.begin();
  aMixer.setReadModeSPI(10);
  Serial.begin(115200); 
  readRecipes();
  aKeypad.begin();
  lcd.begin(16,2);
  output.begin(true,&lcd,16);
  pinMode(A0, OUTPUT);
  testServo.attach(100);
  
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
  SoftwareServo::refresh();
  static char readKey;
  float Grams;
  static int recipeIndex = 0;
  static int motorState = 180;
  if (motorState == 180) {
    motorState = 90;
  } else {
    motorState = 180;
  }
  testServo.write(motorState);
  Grams = aMixer.getGrams();
  output.setCursor(0,0);
  output.output(Grams);
//  output.setCursor(3,0);
  output.outputln(" grams");
  for (int i = 0; i<=9; i++) {
    if (readKey != -1) {    // we didn't just read a key
      readKey = aKeypad.keyScan();
      SoftwareServo::refresh();
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
          output.outputln(" (J/M)");
          break;
        case 'c':
          if (--recipeIndex < 0) {
            recipeIndex = 0;
          }
          output.output("Recipe ");
          output.output(recipeIndex);
          output.outputln(" (J/M)");
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
    delay(25); //50ms * 10 = 1/2 second visual updates, but 50ms keyscans
    SoftwareServo::refresh();
    delay(25);
  }
  readKey = 0;
}
