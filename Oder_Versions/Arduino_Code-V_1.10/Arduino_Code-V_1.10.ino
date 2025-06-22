// --- FUTURES ---
#include "decoder_both.h" 
#include <LiquidCrystal.h>
#include "lcd_display.h"

// --- PINS ---
const int dotPin = A0;                  // "." paddle
const int dashPin = A1;                  // "-" paddle
const int buzzerPin = 8;                // buzzer
const int FunctionButtonPin = 7;        // function button
const int clearButtonPin = 6;           // clear button (ckena the display)
const int morseLEDPin = 10;
const int powerLEDPin = 9;      
const int wpmPotPin = A2;               // WPM pot

// --- WPM settings (Word Per Minutes) ---
int WPM = 20;                   // Default 
int dotDuration = 1200 / WPM;   // unit duration calculation
const int TONE_HZ = 1000;        // default buzzer Hz

// --- VARs --- 
String morseBuffer = "";
String sentenceBuffer = "";
bool lastWasDit = false;
bool justDecoded = false;
int decoderMode = 0;

unsigned long lastKeyTime = 0;
unsigned long lastDecodeTime = 3000  ;
unsigned long lastModeSwitch = 0;

enum KeyingState { IDLE, SENDING_DIT, SENDING_DAH, POST_DELAY };
KeyingState keyingState = IDLE;
unsigned long keyingStart = 0;
int keyingDuration = 0;
char queuedElement = '\0';

void setup() {
  pinMode(dotPin, INPUT_PULLUP);
  pinMode(dashPin, INPUT_PULLUP);
  pinMode(FunctionButtonPin, INPUT_PULLUP);
  pinMode(clearButtonPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
  pinMode(morseLEDPin, OUTPUT);
  pinMode(powerLEDPin, OUTPUT);

  digitalWrite(powerLEDPin, HIGH);

  Serial.begin(9600);
  Serial.println(" ");
  Serial.println("CW KEYER by Ekin Efe GUNGOR");
  

  initLCD();
  updateLCDMode(decoderMode);
}

void loop() {
  unsigned long now = millis();

  // --- WPM from potentiometer ---
  int potValue = analogRead(wpmPotPin);
  WPM = map(potValue, 0, 1023, 5, 35);
  dotDuration = 1200 / WPM;

  static int lastWPM = -1;
  if (WPM != lastWPM) {
    updateLCDMode(decoderMode);
    updateWPMLevel(WPM);
    lastWPM = WPM;
  }

  // --- Mode toggle ---
  if (digitalRead(FunctionButtonPin) == LOW && now - lastModeSwitch > 500) {
    decoderMode = (decoderMode + 1) % 3;
    updateLCDMode(decoderMode);
    lastModeSwitch = now;
  }

  // --- Paddle Reading & State Machine ---
  bool dotPressed = digitalRead(dotPin) == LOW;
  bool dashPressed = digitalRead(dashPin) == LOW;

  if (keyingState == IDLE) {
    if (dotPressed && !dashPressed) {
      queuedElement = '.';
      keyingState = SENDING_DIT;
      keyingStart = now;
      tone(buzzerPin, TONE_HZ);
      digitalWrite(morseLEDPin, HIGH);
      keyingDuration = dotDuration;
      morseBuffer += ".";
      lastKeyTime = now;
      justDecoded = false;
    } 
    else if (dashPressed && !dotPressed) {
      queuedElement = '-';
      keyingState = SENDING_DAH;
      keyingStart = now;
      tone(buzzerPin, TONE_HZ);
      digitalWrite(morseLEDPin, HIGH);
      keyingDuration = dotDuration * 3;
      morseBuffer += "-";
      lastKeyTime = now;
      justDecoded = false;
    } 
    else if (dotPressed && dashPressed) {
      if (lastWasDit) {
        queuedElement = '-';
        keyingDuration = dotDuration * 3;
        morseBuffer += "-";
      } 
      else {
        queuedElement = '.';
        keyingDuration = dotDuration;
        morseBuffer += ".";
      }
      keyingState = (queuedElement == '.') ? SENDING_DIT : SENDING_DAH;
      keyingStart = now;
      tone(buzzerPin, TONE_HZ);
      digitalWrite(morseLEDPin, HIGH);
      lastWasDit = !lastWasDit;
      lastKeyTime = now;
      justDecoded = false;
    }
  } 
  else if (keyingState == SENDING_DIT || keyingState == SENDING_DAH) {
    if (now - keyingStart >= keyingDuration) {
      noTone(buzzerPin);
      digitalWrite(morseLEDPin, LOW);
      keyingState = POST_DELAY;
      keyingStart = now;
      keyingDuration = dotDuration;
    }
  } 
  else if (keyingState == POST_DELAY) {
    if (now - keyingStart >= keyingDuration) {
      keyingState = IDLE;
    }
  }

  // --- Decode Morse after a pause ---
  if (morseBuffer.length() > 0 && !dotPressed && !dashPressed &&
      (now - lastKeyTime >= dotDuration * 3) && !justDecoded) {
    decodeMorse(morseBuffer);
    morseBuffer = "";
    justDecoded = true;
    lastDecodeTime = now;
    Serial.println(sentenceBuffer);
    updateLCDText(sentenceBuffer);
  }

  // --- Word gap ---
  if (justDecoded && !dotPressed && !dashPressed &&
      (now - lastDecodeTime >= dotDuration * 7)) {
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
    updateWPMLevel(WPM);
    updateLCDText(sentenceBuffer);
    Serial.println("=== CLEARED ===");
    delay(500);
  }
}