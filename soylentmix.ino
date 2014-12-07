#include <SPI.h>
#include <HardwareSerial.h>
#include "mixer.h"
#include "keypad.h"

  int readvalue; 
  Mixer aMixer;
  Keypad aKeypad;

void setup(){ 
  aMixer.begin();
  Serial.begin(115200); 
  if (!Serial) {
    while (!Serial) {
      delay(1);
    }
  }
  aKeypad.begin();
}


void loop() {
  char readKey;
  float Grams;
  Grams = aMixer.getGrams();
  Serial.print(Grams);
  Serial.println(" grams");
  for (int i = 0; i<=9; i++) {
    readKey = aKeypad.keyScan();
    if (readKey == 'Z') {
      Serial.println("Resetting...");
      aMixer.resetZero();
    }
    delay(50); //50ms * 10 = 1/2 second visual updates, but 50ms keyscans
  }
}
