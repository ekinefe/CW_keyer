#ifndef DEFAULT_SOUND_H
#define DEFAULT_SOUND_H

#include "PinSettings.h"
#include "Globals.h"
#include "lcd_display.h"
#include "Buffer_Cleanner.h"

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
    delay(1000);
    CleanBuffer();
}

void callSign(){
    encodeBuffer = "TA1TA";         // what to send
    charIndex      = 0;          // start at first character
    symbolIndex    = 0;          // start at first dot/dash
    encodeState    = INTER_LETTER;      // step into the letter-spacing state
    // encodeStartTime = millis(); // record when to begin
    // Serial.print("Encoding: ");

    Serial.println(encodeBuffer); 
}

#endif