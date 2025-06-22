#ifndef BUFFER_CLEANNER_H
#define BUFFER_CLEANNER_H

#include "Globals.h"
#include "lcd_display.h"

void CleanBuffer () {
    sentenceBuffer = "";
    morseBuffer = "";

    updateLCDText(sentenceBuffer);
    Serial.println("sentenceBuffer: === CLEARED ===");
    Serial.println("morseBuffer: === CLEARED ===");
    delay(300);  // debounce
    updateLCDMode(decoderMode);
    updateWPMLevel(WPM);
    updateLCDText(sentenceBuffer);
}

#endif