#ifndef WPM_CONTROLLER_H
#define WPM_CONTROLLER_H

#include <Arduino.h>
#include "PinSettings.h"
#include "Globals.h"
#include "lcd_display.h"

void WPM_Controller() {
  // read the wpm pot
  int potValue = analogRead(wpmPotPin);
  // map to 5–35 WPM
  WPM = map(potValue, 0, 1023, 5, 35);
  // recalc the dot length
  dotDuration = 1200 / WPM;
  // only refresh the LCD when it actually changes
  static int lastWPM = -1;
  if (WPM != lastWPM) {
    updateLCDMode(decoderMode);
    updateWPMLevel(WPM);
    lastWPM = WPM;
  }
}

#endif // WPM_CONTROLLER_H
