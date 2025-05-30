#ifndef BUTTON_CONTROL_H
#define BUTTON_CONTROL_H

#include <Arduino.h>

extern int functionButtonPin;
extern int clearButtonPin;
extern int decoderMode;
extern String sentenceBuffer;
extern void updateLCDMode(int mode);

void initButtons() {
  pinMode(functionButtonPin, INPUT_PULLUP);
  pinMode(clearButtonPin, INPUT_PULLUP);
}

void readButtons() {
  static bool lastFuncState = HIGH;
  static bool lastClearState = HIGH;

  bool funcState = digitalRead(functionButtonPin);
  bool clearState = digitalRead(clearButtonPin);

  if (lastFuncState == HIGH && funcState == LOW) {
    decoderMode = (decoderMode + 1) % 3;
    updateLCDMode(decoderMode);
    delay(250); // debounce
  }

  if (lastClearState == HIGH && clearState == LOW) {
  sentenceBuffer = "";
  Serial.println("Buffer cleared.");
  updateLCDMode(decoderMode); // refresh
  delay(250); // debounce
}

  lastFuncState = funcState;
  lastClearState = clearState;
}

#endif
