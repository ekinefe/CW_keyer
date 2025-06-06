#ifndef PADDLE_INPUT_H
#define PADDLE_INPUT_H

#include <Arduino.h>

// External variables
extern int dotPin;
extern int dashPin;
extern int buzzerPin;
extern int morseLEDPin;
extern int WPM;
extern String currentSymbol;
extern unsigned long lastInputTime;

extern void playDot();
extern void playDash();
extern void stopTone();

// --- State machine states ---
enum KeyingState { IDLE, SENDING_DOT, SENDING_DASH, POST_DELAY };
KeyingState keyingState = IDLE;

char queuedElement = '\0';
bool lastWasDot = false;
unsigned long keyingStartTime = 0;
unsigned long keyingLength = 0;

void startKeying(unsigned long now) {
  keyingStartTime = now;
  keyingState = (queuedElement == '.') ? SENDING_DOT : SENDING_DASH;
}

void initPaddles() {
  pinMode(dotPin, INPUT_PULLUP);
  pinMode(dashPin, INPUT_PULLUP);
}

void handlePaddles() {
  unsigned long now = millis();
  int dotDuration = 1200 / WPM;
  int dashDuration = dotDuration * 3;

  bool dotPressed = digitalRead(dotPin) == LOW;
  bool dashPressed = digitalRead(dashPin) == LOW;

  if (keyingState == IDLE) {
    if (dotPressed && !dashPressed) {
      queuedElement = '.';
      keyingLength = dotDuration;
      playDot();
      currentSymbol += ".";
      startKeying(now);
    }
    else if (dashPressed && !dotPressed) {
      queuedElement = '-';
      // keyingLength = dotDuration * 3;
      keyingLength = dashDuration;
      playDash();
      currentSymbol += "-";
      startKeying(now);
    }
    else if (dotPressed && dashPressed) {
      queuedElement = lastWasDot ? '-' : '.';
      keyingLength = (queuedElement == '.') ? dotDuration : dotDuration * 3;
      if (queuedElement == '.') playDot(); else playDash();
      currentSymbol += queuedElement;
      startKeying(now);
      lastWasDot = !lastWasDot;
    }
    lastInputTime = now;
  }
  else if (keyingState == SENDING_DOT || keyingState == SENDING_DASH) {
    if (now - keyingStartTime >= keyingLength) {
      stopTone();
      keyingStartTime = now;
      keyingState = POST_DELAY;
      keyingLength = dotDuration; // inter-element gap
    }
  }
  else if (keyingState == POST_DELAY && now - keyingStartTime >= keyingLength) {
    keyingState = IDLE;
  }
}

#endif
