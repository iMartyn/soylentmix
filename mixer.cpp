/* 
 * File:   mixer.cpp
 * Author: ranyardm
 *
 * Created on 24 November 2014, 17:52
 */

#include "mixer.h"
#include <SPI.h>
#include <HardwareSerial.h>
using namespace std;

#ifndef ARDUINO
void sleep(int) {}
#endif


Mixer::Mixer () {
  this->currentZeroReading = BASE_ZERO_READING;
  int arr[MAX_RECIPES][MAX_HOPPERS] = RECIPES;
  for (int i=0; i<MAX_HOPPERS; i++) {
    this->hoppers[0] = -1;
    this->ingredients[0] = "Unknown ingredient";
    for (int j=0; j<MAX_RECIPES; j++) {
      this->recipes[j][i] = arr[j][i];
    }
  }
  hoppers[0] = HOPPER_PIN_1;
  hoppers[1] = HOPPER_PIN_2;
  hoppers[2] = HOPPER_PIN_3;
  hoppers[3] = HOPPER_PIN_4;
  ingredients[0] = INGREDIENT_1;
  ingredients[1] = INGREDIENT_2;
  ingredients[2] = INGREDIENT_3;
  ingredients[3] = INGREDIENT_4;
}

void Mixer::resetZero() {
  this->setZero(this->takeReading());
}

void Mixer::setZero(int reading){
  this->currentZeroReading = reading;
}

void Mixer::begin(){
  //Arduino code, yuk, we move initialisers to begin because other stuff might need it not initialised
  this->setReadModeAnalog(DEFAULT_ANALOG_PIN);  
}

void Mixer::setReadModeSPI(int chipSelectPin) {
  this->readModeSPI = true;
  this->SPICsPin = chipSelectPin;
  digitalWrite (this->SPICsPin, HIGH); // Turn off adc until we need it  
  SPI.setClockDivider( SPI_CLOCK_DIV64 ); // slow the SPI bus down
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);    // SPI 0,0 as per MCP330x data sheet 
  SPI.begin();
}

void Mixer::setReadModeAnalog(int pin) {
  this->readModeSPI = false;
  this->analogReadPin = pin;
  pinMode(pin, INPUT);
}

void Mixer::openHopper(int) {
    //TODO yeah, code this
}

void Mixer::closeHopper(int) {
    //TODO yeah, code this
}

void Mixer::pourPowder(int hopper, int weightValue) {
    int startValue = takeAverageReading(DEFAULT_READINGS, DEFAULT_DELAY);
    int targetValue = startValue + weightValue;
    while (takeAverageReading(DEFAULT_READINGS, DEFAULT_DELAY), targetValue) {
        this->openHopper(hopper); // will have no effect if already open
    }
    this->closeHopper(hopper);
}

int Mixer::takeAverageReading(int averageHowManyReadings, int delayBetweenReadings) {
    long total = 0;
    return takeReadingActual();
    for (int i=0;i<=averageHowManyReadings;i++) {
        total += this->takeReadingActual();
        if (i<averageHowManyReadings-1) {
            delay(delayBetweenReadings);
        }
    }
    float ret = total/averageHowManyReadings;
    return (int) ret;
}

int Mixer::takeSPIReading() {
  int b1 = 0, b2 = 0;
  int sign = 0;
  digitalWrite (this->SPICsPin, LOW); // Select adc
  noInterrupts(); //disable interrupts because SPI
  b1 = SPI.transfer(0);       // don't care what we send
  // hi byte will have XX0SBBBB
  // set the top 3 don't care bits so it will format nicely
  b1 &= 0x3f;
  sign = b1 & 0x10;
  int hi = b1 & 0x0f;
  // read low byte
  b2 = SPI.transfer(0);              // don't care what we send
  interrupts(); //re-enable interrupts
 
  int lo = b2;
  digitalWrite(this->SPICsPin, HIGH); // turn off device

  int reading = hi * 256 + lo;
  if (sign) {
    reading -= 0x1000;
  }
  return reading;
}

int Mixer::takeReading() {
    return this->takeAverageReading(DEFAULT_READINGS, DEFAULT_DELAY);
}

int Mixer::takeReadingActual() {
    int reading;
    if (this->readModeSPI) {
        reading = this->takeSPIReading();
    } else {
        reading = analogRead(this->analogReadPin);
    }
    return reading;
}

float Mixer::readingToGrams(int reading) {
    if (reading - this->currentZeroReading < UNITS_PER_GRAM) return 0;
    float lookupIndex [] = READINGS_TO_GRAMS_INDEX;
    float lookupValue [] = READINGS_TO_GRAMS_VALUES;
   /* for (int i=0; i<READINGS_TO_GRAMS_COUNT; i++) {
        if (reading >= lookupIndex[i]) {
            // Apparently the curve is wrong, and 3.16 is about right!
            return (reading-this->currentZeroReading)/lookupValue[i];
        }
    }*/
    float grams = (reading-this->currentZeroReading)/UNITS_PER_GRAM;
    return grams;
}

float Mixer::getGrams() {
  int reading = this->takeReading();
  Serial.print(this->currentZeroReading);
  Serial.print('/');
  Serial.print("Reading ");
  Serial.println(reading);
  return this->readingToGrams(reading);
}

void Mixer::runRecipe(int recipeIndex) {
  int currentReading = 0;
  int originalReading = takeReading();
  int currentGrams = 0;
  for (int i=0; i<MAX_HOPPERS; i++) {
    if (this->recipes[recipeIndex][i]>0) {
      Serial.println("Zeroing...");
      this->resetZero();
      Serial.print("Opening Hopper ");
      Serial.print(i);
      Serial.print(" and pouring ");
      Serial.print(this->recipes[recipeIndex][i]);
      Serial.print(" grams of ");
      Serial.print(this->ingredients[i]);
      Serial.println("...");
      this->openHopper(i);
      currentGrams = 0;
      while (currentGrams < this->recipes[recipeIndex][i]) {
        currentReading = this->takeReading();
        currentGrams = this->readingToGrams(currentReading);
      }
      Serial.println("Reached or overshot!");
      Serial.print("Closing hopper ");
      Serial.print(i);
      Serial.println(" and continuing...");
      this->closeHopper(i);
    }
  }
  this->setZero(originalReading);
}
