/* 
 * File:   mixer.h
 * Author: ranyardm
 *
 * Created on 24 November 2014, 17:57
 */

#ifndef MIXER_H
#define	MIXER_H

#define DEFAULT_READINGS 5 // average over this many
#define DEFAULT_DELAY 5 // in millis
#define MAX_HOPPERS 7

#define BASE_ZERO_READING 817

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

