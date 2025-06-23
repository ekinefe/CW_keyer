#ifndef ENCODER_H
#define ENCODER_H

#include "PinSettings.h"
#include "Globals.h"

// ----------------------------------------------------------------------------
// stopTone(): Stop any ongoing tone on the buzzer and turn off the LED.
// ----------------------------------------------------------------------------
void stopTone() {
  noTone(buzzerPin);
  digitalWrite(morseLEDPin, LOW);
}

// ----------------------------------------------------------------------------
// playDash(): Begin a “dash” element by turning the tone and LED on.
// ----------------------------------------------------------------------------
void playDash() {
  tone(buzzerPin, toneHz);
  digitalWrite(morseLEDPin, HIGH);
}

// ----------------------------------------------------------------------------
// playDot(): Begin a “dot” element by turning the tone and LED on.
// ----------------------------------------------------------------------------
void playDot() {
  tone(buzzerPin, toneHz);
  digitalWrite(morseLEDPin, HIGH);
}

// ----------------------------------------------------------------------------
// getMorseCode(c):  
//   Returns the dot/dash `String` for character `c` based on morseTable[].
//   - A–Z map to indices 0–25
//   - 0–9 map to indices 26–35
//   - Others return an empty string.
// ----------------------------------------------------------------------------
String getMorseCode(char c) {
  if (c >= 'A' && c <= 'Z') return morseTable[c - 'A'];
  if (c >= '0' && c <= '9') return morseTable[c - '0' + 26];
  return "";
}

// ----------------------------------------------------------------------------
// handleEncoding(now):
//   Main serial-to-Morse state machine. Call this each loop with `now = millis()`.
//   - Reads new text from Serial when idle
//   - Converts characters to Morse using getMorseCode()
//   - Drives playDot(), playDash(), stopTone(), and LED timing
//   - Manages states: IDLE_ENCODE, INTER_LETTER, WORD_GAP, ELEMENT_PLAY,
//     INTER_ELEMENT.
// ----------------------------------------------------------------------------
void handleEncoding(unsigned long now) {
  static unsigned long elementStart = 0;
  static bool isPlaying = false;          // isPlaying: true if buzzer/LED are currently active
  static char symbol = '\0';

  // 1) NEW INPUT: If idle and Serial has data, load it
  if (encodeState == IDLE_ENCODE && Serial.available()) {
    encodeBuffer = Serial.readStringUntil('\n');  // Read until newline
    encodeBuffer.trim();                          // Remove whitespace
    encodeBuffer.toUpperCase();                   // Work in uppercase
    charIndex = 0;                                // Reset char position
    symbolIndex = 0;                              // Reset symbol position
    encodeState = INTER_LETTER;                   // Start inter-letter gap
    encodeStartTime = now;                        // Mark current time
    Serial.print("Encoding: ");                   
    Serial.println(encodeBuffer);                 // Debug output
    return;                                       // Defer playback until next call
  }
  // 2) DONE? If we've sent all chars, return to idle
  if (charIndex >= encodeBuffer.length()) {
    encodeState = IDLE_ENCODE;
    return;
  }

  // 3) Get current character and branch on state
  char currentChar = encodeBuffer.charAt(charIndex);
  switch (encodeState) {

    // --- INTER_LETTER: Wait dotDuration*3 before next letter or word gap
    case INTER_LETTER:
      if (now - encodeStartTime >= dotDuration * 3) {
        if (currentChar == ' ') {
          encodeState = WORD_GAP;
        } else {
          currentMorse = getMorseCode(currentChar);
          symbolIndex = 0;
          encodeState = ELEMENT_PLAY;
        }
        encodeStartTime = now;
      }
    break;
    
    // --- WORD_GAP: Wait dotDuration*7 before resuming after a space
    case WORD_GAP:
      if (now - encodeStartTime >= dotDuration * 7) {
        charIndex++;
        encodeState = INTER_LETTER;
        encodeStartTime = now;
      }
    break;

    // --- ELEMENT_PLAY: Play a dot or dash from currentMorse
    case ELEMENT_PLAY:
      if (symbolIndex < currentMorse.length()) {
        symbol = currentMorse.charAt(symbolIndex);
        if (symbol == '.') {
          playDot();
          keyingLength = dotDuration;
        } else if (symbol == '-') {
          playDash();
          keyingLength = dotDuration * 3;
        }
        elementStart = now;
        encodeState = INTER_ELEMENT;  // Move to wait-and-stop
        isPlaying = true;
      } else {
        charIndex++;
        encodeState = INTER_LETTER;
        encodeStartTime = now;
      }
    break;

    // --- INTER_ELEMENT: Short gap (1 unit) between elements
    case INTER_ELEMENT:
      if (isPlaying && now - elementStart >= keyingLength) {
        stopTone();  // Stop after dot/dash
        isPlaying = false;
        elementStart = now;
      } else if (!isPlaying && now - elementStart >= dotDuration) {
        symbolIndex++;
        if (symbolIndex < currentMorse.length()) {
          encodeState = ELEMENT_PLAY;
        } else {
          charIndex++;
          encodeState = INTER_LETTER;
          encodeStartTime = now;
        }
      }
    break;
  }
}

#endif