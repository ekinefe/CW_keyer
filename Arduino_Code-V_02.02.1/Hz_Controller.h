#ifndef HZ_CONTROLLER_H
#define HZ_CONTROLLER_H

#include <Arduino.h>
#include "PinSettings.h"
#include "Globals.h"
#include "lcd_display.h"

void hzController() {
    // const int minHz = 400;
    // const int maxHz = 1500;
    // const int stepHz = 10;
    // const int numSteps = (maxHz - minHz) / stepHz;  // =110

    int potValue = analogRead(HzPotPin);
    int step = map(potValue, 0, 1023, 0, numSteps);
    newToneHz = minHz + step * stepHz;

    lastToneHz = -1;
    if (newToneHz != lastToneHz) {
        toneHz = newToneHz;
        updateLCDToneHz(toneHz);
        lastToneHz = newToneHz;
    }
}


#endif