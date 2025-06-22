#ifndef BUTTON_CONTROLLER_H
#define BUTTON_CONTROLLER_H

#include "PinSettings.h"
#include "Globals.h"
#include "Buffer_Cleanner.h"
#include <Arduino.h>

void clearButton(){
    // sentenceBuffer = "";
    // morseBuffer = "";
    // justDecoded = false;
    // lcd.clear();
    // updateLCDMode(decoderMode);
    // updateWPMLevel(WPM);
    // updateLCDText(sentenceBuffer);
    // Serial.println("=== CLEARED ===");
    // delay(300);  // debounce
    CleanBuffer();
}

// void modeButton() {

// }

#endif
