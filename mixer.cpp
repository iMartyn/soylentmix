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
    void pourPowder(int,int);
    void openHopper(int);
    void closeHopper(int);
};

Mixer::Mixer () {
    srand (time(NULL));
    hoppers[0] = 1;
    hoppers[1] = 2;
    hoppers[2] = 3;
    hoppers[3] = 4;
    hoppers[4] = 5;
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
/*
 * 
 */
int main(int argc, char** argv) {
    Mixer aMixer;
    int avg = aMixer.takeAverageReading(4,1);
    return 0;
}

