// read the MCP3304 in quad differential mode, non-bit-banging version
//#include <Serial.h>
#include <SPI.h>
#include <HardwareSerial.h>
#include "mixer.h"
#include "keypad.h"

long ticks = 0;


// for Leonardo
#define SELPIN 10    // chip-select
#define ZEROBUTTON A1

int readvalue; 

  Mixer aMixer;
  Keypad aKeypad;

void setup(){ 
  /*debug/example version 
  //set pin modes 
  pinMode(SELPIN, OUTPUT); 

  // disable device to start with 
  digitalWrite(SELPIN, HIGH);
  SPI.setClockDivider( SPI_CLOCK_DIV64 ); // slow the SPI bus down
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);    // SPI 0,0 as per MCP330x data sheet 
  SPI.begin(); */
  /* use the class */
  aMixer.begin();
  Serial.begin(115200); 
  if (!Serial) {
    while (!Serial) {
      delay(1);
    }
  }
  aKeypad.begin();
  pinMode(ZEROBUTTON, INPUT_PULLUP);

}


void loop() {
  aKeypad.debugMap();
  char readKey;
  readKey = aKeypad.keyScan();
  if (readKey != 0) {
    Serial.println(readKey);
  }
  delay(5);
//  aKeypad.debugMap();
  /*  Use the class 
  float Grams = aMixer.getGrams();
  Serial.print(Grams);
  Serial.println(" grams");
  for (int i = 0; i<=9; i++) {
    if (digitalRead(ZEROBUTTON) == LOW) {
      Serial.println("Resetting...");
      aMixer.resetZero();
    }
    delay(50);
  }
  /* use example/debug code
  int A = read_adc(0);
  delay(500); */
}

// non-bit-banging version
// channel ranges from 1.. 3 (not zero!)
// maximum clock frequency is 2.1 MHz @ 5V
//
// this is SPI_CLOCK_DIV8
// at DIV64, 512 in 60ms (8.5 ksps)
// at DIV32, 512 in 35ms (14.6 ksps)
// at DIV16, 512 in 22ms (23.3 ksps)
// at DIV8, 512 in 16ms (32 ksps)

int read_adc(int channel){
  int adcvalue = 0;
  int b1 = 0, b2 = 0;
  int sign = 0;
  static int error = 0, mean = 0;

  // command bits for MCP3304
  // 0000 = diff, ch0 = in+, ch1 = in-
  // 0010 = diff, ch2 = in+, ch3 = in-
  // 0100 = diff, ch4 = in+, ch5 = in-

  digitalWrite (SELPIN, LOW); // Select adc
  noInterrupts();
  b1 = SPI.transfer(0);       // send out second byte of command bits

  // hi byte will have XX0SBBBB
  // set the top 3 don't care bits so it will format nicely
  b1 &= 0x3f;
//  Serial.print(b1, HEX); Serial.print(" ");
  sign = b1 & 0x10;
  int hi = b1 & 0x0f;
//  Serial.print(b1, HEX); Serial.print(" ");

  // read low byte
  b2 = SPI.transfer(0);              // don't care what we send
//  Serial.println(b2, HEX);
  interrupts();
 
  int lo = b2;
  digitalWrite(SELPIN, HIGH); // turn off device

  int reading = hi * 256 + lo;
 

  if (sign) {
    reading -= 0x1000;
  }
//  reading = analogRead(A0);
  mean = (mean + reading) /2;
  int e = abs(mean - reading)*10;
  error = ((error * 9) + e)/10;
  Serial.print(mean);
  Serial.print(' ');
  Serial.print(reading);
  Serial.print(' ');
  Serial.println(error);
  Serial.print(aMixer.readingToGrams(reading));
  Serial.println(" Gramms");
  return (mean);
}
