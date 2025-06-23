#ifndef WPM_CONTROLLER_H
#define WPM_CONTROLLER_H

#include <Arduino.h>
#include "PinSettings.h"
#include "Globals.h"
#include "lcd_display.h"

void WPM_Controller() {
  int potValue = analogRead(wpmPotPin);   // read the wpm pot
  WPM = map(potValue, 0, 1023, 5, 35);    // map to 5–35 WPM
  dotDuration = 1200 / WPM;               // recalc the dot length
  lastWPM = -1;                // only refresh the LCD when it actually changes
  if (WPM != lastWPM) {
    updateLCDMode(decoderMode);
    updateWPMLevel(WPM);
    lastWPM = WPM;
  }
}

#endif
