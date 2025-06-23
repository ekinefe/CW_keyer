#ifndef BUFFER_CLEANNER_H
#define BUFFER_CLEANNER_H

#include "Globals.h"
#include "lcd_display.h"

// ----------------------------------------------------------------------------
// Clears both the sentence and Morse-code buffers, refreshes the LCD, and
// reapplies the current decoder mode and WPM settings with a debounce delay.
// ----------------------------------------------------------------------------

void CleanBuffer () {
    sentenceBuffer = "";    // RESET the text buffer.
    morseBuffer = "";       // RESER the morsse-Code buffer.

    // Informamtion output for CLI
    Serial.print("sentenceBuffer: === CLEARED ===");
    Serial.print(" || ");
    Serial.println("morseBuffer: === CLEARED ===");
    
    delay(300);  // debounce

    // Update all the elemts on the LCD
    clearLCDText();
    updateLCDMode(decoderMode);
    updateWPMLevel(WPM);
    updateLCDText(sentenceBuffer);
}

#endif