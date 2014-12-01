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
/*    hoppers[0] = HOPPER_PIN_1;
    hoppers[1] = HOPPER_PIN_2;
    hoppers[2] = HOPPER_PIN_3;
    hoppers[3] = HOPPER_PIN_4;
    hoppers[4] = HOPPER_PIN_5;*/
}

void Mixer::resetZero() {
  this->currentZeroReading = this->takeReading();
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
        return reading;
    } else {
        return analogRead(this->analogReadPin);
    }
}

float Mixer::readingToGrams(int reading) {
    if (reading - BASE_ZERO_READING < 3.1) return 0;
    float lookupIndex [] = READINGS_TO_GRAMS_INDEX;
    float lookupValue [] = READINGS_TO_GRAMS_VALUES;
    for (int i=0; i<READINGS_TO_GRAMS_COUNT; i++) {
        if (reading >= lookupIndex[i]) {
            // Apparently the curve is wrong, and 3.16 is about right!
            return (reading-BASE_ZERO_READING)/3.16;
            return (reading-BASE_ZERO_READING)/lookupValue[i];
        }
    }
}

float Mixer::getGrams() {
  int reading = this->takeReading();
  return this->readingToGrams(reading);
}
