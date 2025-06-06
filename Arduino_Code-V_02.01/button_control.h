// #ifndef BUTTON_CONTROL_H
// #define BUTTON_CONTROL_H

// #include <Arduino.h>

// extern int functionButtonPin;
// extern int clearButtonPin;
// extern int decoderMode;
// extern String sentenceBuffer;
// extern void updateLCDMode(int mode);

// void initButtons() {
//   pinMode(functionButtonPin, INPUT_PULLUP);
//   pinMode(clearButtonPin, INPUT_PULLUP);
// }

// void readButtons() {
//   static bool lastFuncState = HIGH;
//   static bool lastClearState = HIGH;

//   bool funcState = digitalRead(functionButtonPin);
//   bool clearState = digitalRead(clearButtonPin);

//   if (lastFuncState == HIGH && funcState == LOW) {
//     decoderMode = (decoderMode + 1) % 3;
//     updateLCDMode(decoderMode);
//     delay(250); // debounce
//   }

//   if (lastClearState == HIGH && clearState == LOW) {
//     sentenceBuffer = "";
//     Serial.println("Buffer cleared.");
//     updateLCDMode(decoderMode); // refresh
//     delay(250); // debounce
//   }

//   lastFuncState = funcState;
//   lastClearState = clearState;
// }

// void handleClearButton() {
//   static bool lastState = HIGH;
//   bool currentState = digitalRead(clearButtonPin);

//   if (lastState == HIGH && currentState == LOW) {
//     sentenceBuffer = "";
//     currentSymbol = "";
//     lcd.clear();
//     lcd.setCursor(0, 0);
//     lcd.print("Buffer Cleared");
//     Serial.println("Buffer Cleared");
//     delay(300);
//   }

//   lastState = currentState;
// }


// void updateModeButton() {
//   static bool lastState = HIGH;
//   bool currentState = digitalRead(functionButtonPin);

//   if (lastState == HIGH && currentState == LOW) {
//     mode = (mode + 1) % 3;
//     updateLCDMode(mode);
//     Serial.print("Mode changed to: ");
//     Serial.println(mode);
//   }

//   lastState = currentState;
// }

// #endif

#ifndef BUTTON_CONTROL_H
#define BUTTON_CONTROL_H

#include <Arduino.h>

extern int functionButtonPin;
extern int clearButtonPin;
extern int decoderMode;
extern String sentenceBuffer;
extern int buzzerPin;

void updateModeButton() {
  static bool lastState = HIGH;
  bool currentState = digitalRead(functionButtonPin);

  if (lastState == HIGH && currentState == LOW) {
    decoderMode = (decoderMode + 1) % 3;
  }
  lastState = currentState;
}

void clearBufferButton() {
  static bool lastClearState = HIGH;
  bool currentClearState = digitalRead(clearButtonPin);

  if (lastClearState == HIGH && currentClearState == LOW) {
    sentenceBuffer = "";
  }
  lastClearState = currentClearState;
}

// void FButton1() {
//   playCQ(); // Default "CQ"
// }

// void FButton2() {

// }

// void FButton3() {

// }

// void FButton4() {

// }

// void FButton5() {

// }

#endif
