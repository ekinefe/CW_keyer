#ifndef DEFAULT_SOUND_H
#define DEFAULT_SOUND_H

#include "PinSettings.h"
#include "Globals.h"
#include "lcd_display.h"
#include "Buffer_Cleanner.h"

// ----------------------------------------------------------------------------
// Send the standard “CQ” call sign.
// - Loads “CQ” into the encode buffer and sentence buffer (for display).
// - Resets character/symbol indices and steps into inter-letter spacing.
// - Updates the LCD to show “CQ” at current mode and WPM.
// - Prints to Serial for monitoring, waits 1 s, then clears buffers.
// ----------------------------------------------------------------------------

void CQ(){
    encodeBuffer = "CQ";         // what to send
    sentenceBuffer = encodeBuffer;
    charIndex      = 0;          // start at first character
    symbolIndex    = 0;          // start at first dot/dash
    encodeState    = INTER_LETTER;      // step into the letter-spacing state
    // encodeStartTime = millis(); // record when to begin
    // Serial.print("Encoding: "); 
    updateLCDMode(decoderMode);
    updateWPMLevel(WPM);
    updateLCDText(sentenceBuffer);
    Serial.println(encodeBuffer); 
    delay(2000);
    // CleanBuffer();
}

// ----------------------------------------------------------------------------
// Send the device’s call sign “TA1TA” over Serial.
// - Loads “TA1TA” into encodeBuffer, resets indices, steps into inter-letter.
// - (LCD and CleanBuffer steps are omitted here; just serial output.)
// ----------------------------------------------------------------------------
void callSign(){
    encodeBuffer = "TA1TA";         // what to send
    charIndex      = 0;          // start at first character
    symbolIndex    = 0;          // start at first dot/dash
    encodeState    = INTER_LETTER;      // step into the letter-spacing state
    // encodeStartTime = millis(); // record when to begin
    // Serial.print("Encoding: ");

    updateLCDMode(decoderMode);
    updateWPMLevel(WPM);
    updateLCDText(sentenceBuffer);
    Serial.println(encodeBuffer); 
    delay(2500);
    // CleanBuffer();
}

#endif