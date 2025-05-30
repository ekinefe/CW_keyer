#ifndef WPM_CONTROL_H
#define WPM_CONTROL_H

#include <Arduino.h>

extern int wpmPotPin;
extern int WPM;
extern void updateWPMLevel(int);

void updateWPM() {
  int potValue = analogRead(wpmPotPin);
  WPM = map(potValue, 0, 1023, 5, 40);
  updateWPMLevel(WPM);
}

#endif
