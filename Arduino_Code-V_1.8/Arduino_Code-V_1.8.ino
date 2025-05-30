// --- Decoder Mode Selection ---
#include "decoder_both.h"  // Uses mode switching dynamically

#include <LiquidCrystal.h>
#include "lcd_display.h"

// --- Pin Assignments ---
const int ditPin = A0;
const int dahPin = A1;
const int buzzerPin = 8;
const int buttonPin = 7;         // mode toggle button
const int clearButtonPin = 6;    // clear message button
const int wpmPotPin = A2;        // potentiometer to control WPM

// --- State and Timing ---
int WPM = 20;
int ditDuration = 1200 / WPM;
const int TONE_HZ = 600;

String morseBuffer = "";
String sentenceBuffer = "";
bool lastWasDit = false;
bool justDecoded = false;
int decoderMode = 0;

unsigned long lastKeyTime = 0;
unsigned long lastDecodeTime = 0;
unsigned long lastModeSwitch = 0;

enum KeyingState { IDLE, SENDING_DIT, SENDING_DAH, POST_DELAY };
KeyingState keyingState = IDLE;
unsigned long keyingStart = 0;
int keyingDuration = 0;
char queuedElement = '\0';

void setup() {
  pinMode(ditPin, INPUT_PULLUP);
  pinMode(dahPin, INPUT_PULLUP);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(clearButtonPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);

  Serial.begin(9600);
  Serial.println();
  Serial.println("CW KEYER by Ekin Efe GUNGOR");

  initLCD();
  updateLCDMode(decoderMode);
}

void loop() {
  unsigned long now = millis();

  // --- WPM from potentiometer ---
  int potValue = analogRead(wpmPotPin);
  WPM = map(potValue, 0, 1023, 5, 35);
  ditDuration = 1200 / WPM;

  static int lastWPM = -1;
  if (WPM != lastWPM) {
    updateLCDMode(decoderMode);
    lastWPM = WPM;
  }

  // --- Mode toggle ---
  if (digitalRead(buttonPin) == LOW && now - lastModeSwitch > 500) {
    decoderMode = (decoderMode + 1) % 3;
    updateLCDMode(decoderMode);
    lastModeSwitch = now;
  }

  // --- Paddle Reading & State Machine ---
  bool ditPressed = digitalRead(ditPin) == LOW;
  bool dahPressed = digitalRead(dahPin) == LOW;

  if (keyingState == IDLE) {
    if (ditPressed && !dahPressed) {
      queuedElement = '.';
      keyingState = SENDING_DIT;
      keyingStart = now;
      tone(buzzerPin, TONE_HZ);
      keyingDuration = ditDuration;
      morseBuffer += ".";
      lastKeyTime = now;
      justDecoded = false;
    } else if (dahPressed && !ditPressed) {
      queuedElement = '-';
      keyingState = SENDING_DAH;
      keyingStart = now;
      tone(buzzerPin, TONE_HZ);
      keyingDuration = ditDuration * 3;
      morseBuffer += "-";
      lastKeyTime = now;
      justDecoded = false;
    } else if (ditPressed && dahPressed) {
      if (lastWasDit) {
        queuedElement = '-';
        keyingDuration = ditDuration * 3;
        morseBuffer += "-";
      } else {
        queuedElement = '.';
        keyingDuration = ditDuration;
        morseBuffer += ".";
      }
      keyingState = (queuedElement == '.') ? SENDING_DIT : SENDING_DAH;
      keyingStart = now;
      tone(buzzerPin, TONE_HZ);
      lastWasDit = !lastWasDit;
      lastKeyTime = now;
      justDecoded = false;
    }
  } else if (keyingState == SENDING_DIT || keyingState == SENDING_DAH) {
    if (now - keyingStart >= keyingDuration) {
      noTone(buzzerPin);
      keyingState = POST_DELAY;
      keyingStart = now;
      keyingDuration = ditDuration;
    }
  } else if (keyingState == POST_DELAY) {
    if (now - keyingStart >= keyingDuration) {
      keyingState = IDLE;
    }
  }

  // --- Decode Morse after a pause ---
  if (morseBuffer.length() > 0 && !ditPressed && !dahPressed &&
      (now - lastKeyTime >= ditDuration * 3) && !justDecoded) {
    decodeMorse(morseBuffer);
    morseBuffer = "";
    justDecoded = true;
    lastDecodeTime = now;
    Serial.println(sentenceBuffer);
    updateLCDText(sentenceBuffer);
  }

  // --- Word gap ---
  if (justDecoded && !ditPressed && !dahPressed &&
      (now - lastDecodeTime >= ditDuration * 7)) {
    sentenceBuffer += " ";
    justDecoded = false;
    updateLCDText(sentenceBuffer);
  }

  // --- Print final only when clear button is pressed ---
  if (digitalRead(clearButtonPin) == LOW) {
    sentenceBuffer = "";
    morseBuffer = "";
    justDecoded = false;
    lcd.clear();
    updateLCDMode(decoderMode);
    updateLCDText(sentenceBuffer);
    Serial.println("=== CLEARED ===");
    delay(500);
  }
}