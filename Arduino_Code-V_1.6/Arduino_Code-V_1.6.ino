#include "decoder.h"

const int ditPin = 2;
const int dahPin = 3;
const int buzzerPin = 9;
const int WPM = 20;
const int TONE_HZ = 600;

unsigned int ditDuration;
bool lastWasDit = false;
String morseBuffer = "";
unsigned long lastInputTime = 0;
const unsigned long letterGap = 1200 / WPM * 3;

void setup() {
  pinMode(ditPin, INPUT_PULLUP);
  pinMode(dahPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
  Serial.begin(9600);
  ditDuration = 1200 / WPM;
  Serial.println("CW Keyer Modularized Ready");
}

void loop() {
  bool ditPressed = digitalRead(ditPin) == LOW;
  bool dahPressed = digitalRead(dahPin) == LOW;

  if (ditPressed && !dahPressed) {
    sendDit();
    morseBuffer += ".";
    lastInputTime = millis();
  } else if (dahPressed && !ditPressed) {
    sendDah();
    morseBuffer += "-";
    lastInputTime = millis();
  } else if (ditPressed && dahPressed) {
    if (lastWasDit) {
      sendDah();
      morseBuffer += "-";
    } else {
      sendDit();
      morseBuffer += ".";
    }
    lastWasDit = !lastWasDit;
    lastInputTime = millis();
  }

  if (morseBuffer.length() > 0 && millis() - lastInputTime > letterGap) {
    char decoded = decodeMorse(morseBuffer);
    Serial.print("  → ");
    Serial.println(decoded);
    morseBuffer = "";
  }

  delay(10);
}

void sendDit() {
  Serial.print(".");
  tone(buzzerPin, TONE_HZ);
  delay(ditDuration);
  noTone(buzzerPin);
  delay(ditDuration);
}

void sendDah() {
  Serial.print("-");
  tone(buzzerPin, TONE_HZ);
  delay(ditDuration * 3);
  noTone(buzzerPin);
  delay(ditDuration);
}
