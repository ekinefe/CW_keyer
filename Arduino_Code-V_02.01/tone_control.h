#ifndef TONE_CONTROL_H
#define TONE_CONTROL_H

#include <Arduino.h>

extern int buzzerPin;
extern int morseLEDPin;
extern int WPM;

void initTone() {
  pinMode(buzzerPin, OUTPUT);
  pinMode(morseLEDPin, OUTPUT);
}

void playDot() {
  int unit = 1200 / WPM;
  digitalWrite(morseLEDPin, HIGH);
  tone(buzzerPin, 600);
  delay(unit);
  noTone(buzzerPin);
  digitalWrite(morseLEDPin, LOW);
  delay(unit);
}

void playDash() {
  int unit = 1200 / WPM;
  digitalWrite(morseLEDPin, HIGH);
  tone(buzzerPin, 600);
  delay(unit * 3);
  noTone(buzzerPin);
  digitalWrite(morseLEDPin, LOW);
  delay(unit);
}

void stopTone() {
  noTone(buzzerPin);
  digitalWrite(morseLEDPin, LOW);
}

void playElement(bool isDot) {
  int unit = 1200 / WPM;
  int duration = unit;

  digitalWrite(morseLEDPin, HIGH);
  tone(buzzerPin, 600);  // Or your `toneFreq`
  delay(duration);
  noTone(buzzerPin);
  digitalWrite(morseLEDPin, LOW);
}



#endif
