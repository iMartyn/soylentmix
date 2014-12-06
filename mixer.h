/* 
 * File:   mixer.h
 * Author: ranyardm
 *
 * Created on 24 November 2014, 17:57
 */

#ifndef MIXER_H
#define	MIXER_H
#include "Arduino.h"


#define MAX_HOPPERS 7
#define DEFAULT_SPI_CS_PIN 10
#define DEFAULT_ANALOG_PIN A0
#define BASE_ZERO_READING 456
// differential on SPI chip
//#define UNITS_PER_GRAM 3.1
// via analog pin
#define UNITS_PER_GRAM 1.019

class Mixer {
    // The pins for the servos on the hoppers
    int hoppers[MAX_HOPPERS];
    int currentZeroReading = BASE_ZERO_READING;
    boolean readModeSPI = true;
    int analogReadPin = -1;
    int SPICsPin = DEFAULT_SPI_CS_PIN;
    int takeSPIReading();
    int takeReadingActual();
public:
    Mixer();
    void begin();
    void resetZero();
    void setReadModeSPI(int);
    void setReadModeAnalog(int);
    float getGrams();
    int takeReading();
    int takeAverageReading(int,int);
    float readingToGrams(int);
    void pourPowder(int,int);
    void openHopper(int);
    void closeHopper(int);
};

#define DEFAULT_READINGS 10 // average over this many
#define DEFAULT_DELAY 1 // in millis


// Correction "curve" for readings to grams
#define READINGS_TO_GRAMS_COUNT 10
#define READINGS_TO_GRAMS_INDEX {817,825,832,851,889,927,969,1007,1577,1957}
#define READINGS_TO_GRAMS_VALUES {3.8,3.7,3.6,3.5,3.4,3.3,3.2,3.18,3.16,3.13}

#define HOPPER_PIN_1 UNDEFINED
#define HOPPER_PIN_2 UNDEFINED
#define HOPPER_PIN_3 UNDEFINED
#define HOPPER_PIN_4 UNDEFINED
#define HOPPER_PIN_5 UNDEFINED


#endif	/* MIXER_H */

