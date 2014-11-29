/* 
 * File:   mixer.cpp
 * Author: ranyardm
 *
 * Created on 24 November 2014, 17:52
 */

#include <cstdlib>
#include <time.h>
#include "mixer.h"

using namespace std;

#ifndef ARDUINO
void sleep(int) {}
#endif

class Mixer {
    // The pins for the servos on the hoppers
    int hoppers[MAX_HOPPERS];
public:
    Mixer();
    int takeReading();
    int takeAverageReading(int,int);
    float readingToGrams(int);
    void pourPowder(int,int);
    void openHopper(int);
    void closeHopper(int);
};

Mixer::Mixer () {
    srand (time(NULL));
    hoppers[0] = HOPPER_PIN_1;
    hoppers[1] = HOPPER_PIN_2;
    hoppers[2] = HOPPER_PIN_3;
    hoppers[3] = HOPPER_PIN_4;
    hoppers[4] = HOPPER_PIN_5;
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
    int total = 0;
    for (int i=0;i<averageHowManyReadings;i++) {
        total += this->takeReading();
        if (i<averageHowManyReadings-1) {
            sleep(delayBetweenReadings);
        }
    }
    return total/averageHowManyReadings;
}

int Mixer::takeReading() {
    int val = rand() % 500 + 50;
    return val;
}

float Mixer::readingToGrams(int reading) {
    if (reading < 1) return 0;
    float lookupIndex [] = READINGS_TO_GRAMS_INDEX;
    float lookupValue [] = READINGS_TO_GRAMS_VALUES;
    for (int i=0; i<READINGS_TO_GRAMS_COUNT; i++) {
        if (reading >= lookupIndex[i]) {
            return reading*lookupValue[i];
        }
    }
}

int main(int argc, char** argv) {
    Mixer aMixer;
    int avg = aMixer.takeAverageReading(4,1);
    sprintf("Average: %i  Grams: %f",avg, aMixer.readingToGrams(avg));
    return 0;
}

