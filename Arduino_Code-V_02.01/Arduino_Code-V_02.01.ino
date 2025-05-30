#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
#include "lcd_display.h"
#include "cli_display.h"
#include "decoder_both.h"      // or char_only / morse_only
#include "paddle_input.h"
#include "tone_control.h"
#include "button_control.h"
#include "wpm_control.h"
#include "startup_sound.h"

extern int WPM;
extern int buzzerPin;
extern String sentenceBuffer;

///////////////////////////////
//      GLOBAL VARIABLES     //
///////////////////////////////

// --- Pins ---
int dotPin = A0;
int dashPin = A1;
int buzzerPin = 8;
int morseLEDPin = 10;
int powerLEDPin = 9;
int functionButtonPin = 7;
int clearButtonPin = 6;
int wpmPotPin = A2;

// --- State ---
int WPM = 20;
int decoderMode = 0;
String sentenceBuffer = "";
String currentSymbol = "";
unsigned long lastInputTime = 0;
bool dotMemory = false;
bool dashMemory = false;

void setup() {
  Serial.begin(9600);
  Serial.println();
  Serial.println("CW KEYER by EKIN EFE GUNGOR   -   V_02.01");
  pinMode(powerLEDPin, OUTPUT);
  digitalWrite(powerLEDPin, HIGH);

  initLCD();
  initPaddles();
  initButtons();
  initTone();
  playStartupSound();

}

void loop() {
  int ditTime = 1200 / WPM;
  updateWPM();
  readPaddles();

  if (dotMemory && dashMemory) {
    playElement(true); delay(ditTime);
    playElement(false); delay(ditTime);
    dotMemory = dashMemory = false;
  } 
  else if (dotMemory) {
    dotMemory = false;
    playElement(true); delay(ditTime);
  } 
  else if (dashMemory) {
    dashMemory = false;
    playElement(false); delay(ditTime);
  }
}


void readPaddles() {
  if (digitalRead(dotPin) == LOW) {
    dotMemory = true;
  }
  if (digitalRead(dashPin) == LOW) {
    dashMemory = true;
  }
}

// void playElement(bool isDot) {
//   int unit = 1200 / WPM;
//   delay(unit);
//   int duration = unit;

//   digitalWrite(morseLEDPin, HIGH);
//   tone(buzzerPin, 600);  // Or your `toneFreq`
//   delay(duration);
//   noTone(buzzerPin);
//   digitalWrite(morseLEDPin, LOW);
// }