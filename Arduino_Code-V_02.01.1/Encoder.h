#ifndef ENCODER_H
#define ENCODER_H

#include "PinSettings.h"
#include "Globals.h"

void stopTone() {
  noTone(buzzerPin);
  digitalWrite(morseLEDPin, LOW);
}

void playDash() {
  tone(buzzerPin, toneHz);
  digitalWrite(morseLEDPin, HIGH);
}

void playDot() {
  tone(buzzerPin, toneHz);
  digitalWrite(morseLEDPin, HIGH);
}

String getMorseCode(char c) {
  if (c >= 'A' && c <= 'Z') return morseTable[c - 'A'];
  if (c >= '0' && c <= '9') return morseTable[c - '0' + 26];
  return "";
}

// --- Serial Encoding Logic ---
void handleEncoding(unsigned long now) {
  static unsigned long elementStart = 0;
  static bool isPlaying = false;
  static char symbol = '\0';

  // Start new input
  if (encodeState == IDLE_ENCODE && Serial.available()) {
    encodeBuffer = Serial.readStringUntil('\n');
    encodeBuffer.trim();
    encodeBuffer.toUpperCase();
    charIndex = 0;
    symbolIndex = 0;
    encodeState = INTER_LETTER;
    encodeStartTime = now;
    Serial.print("Encoding: ");
    Serial.println(encodeBuffer);
    return;
  }

  if (charIndex >= encodeBuffer.length()) {
    encodeState = IDLE_ENCODE;
    return;
  }

  char currentChar = encodeBuffer.charAt(charIndex);

  switch (encodeState) {

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

    case WORD_GAP:
      if (now - encodeStartTime >= dotDuration * 7) {
        charIndex++;
        encodeState = INTER_LETTER;
        encodeStartTime = now;
      }
      break;

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