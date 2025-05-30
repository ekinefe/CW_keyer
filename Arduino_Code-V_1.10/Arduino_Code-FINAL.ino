
// --- Decoder Selection ---
#include "decoder_both.h"

#include <LiquidCrystal.h>
#include "lcd_display.h"

// --- Pin Definitions ---
const int dotPin          = A0;
const int dashPin         = A1;
const int buzzerPin       = 8;
const int morseLEDPin     = 10;
const int powerLEDPin     = 9;
const int wpmPotPin       = A2;
const int functionButton  = 7;
const int clearButton     = 6;

// --- Morse Timing Settings ---
int WPM = 20;
int dotDuration = 1200 / WPM;
const int toneHz = 1000;

// --- State Variables ---
String morseBuffer = "";
String sentenceBuffer = "";
bool lastWasDot = false;
bool justDecoded = false;
int decoderMode = 0;

unsigned long lastKeyTime = 0;
unsigned long lastDecodeTime = 0;
unsigned long lastModeSwitch = 0;

enum KeyingState { IDLE, SENDING_DOT, SENDING_DASH, POST_DELAY };
KeyingState keyingState = IDLE;
unsigned long keyingStart = 0;
int keyingDuration = 0;
char queuedElement = '\0';

// --- DOT & DASH Playback ---
void playDot() {
  tone(buzzerPin, toneHz);
  digitalWrite(morseLEDPin, HIGH);
  delay(dotDuration);
  noTone(buzzerPin);
  digitalWrite(morseLEDPin, LOW);
  delay(dotDuration);
}

void playDash() {
  tone(buzzerPin, toneHz);
  digitalWrite(morseLEDPin, HIGH);
  delay(dotDuration * 3);
  noTone(buzzerPin);
  digitalWrite(morseLEDPin, LOW);
  delay(dotDuration);
}

// --- Morse Encoder (String to be played as CW) ---
void playMorse(String morse) {
  for (char c : morse) {
    if (c == '.') playDot();
    else if (c == '-') playDash();
  }
}

// --- Setup ---
void setup() {
  pinMode(dotPin, INPUT_PULLUP);
  pinMode(dashPin, INPUT_PULLUP);
  pinMode(functionButton, INPUT_PULLUP);
  pinMode(clearButton, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
  pinMode(morseLEDPin, OUTPUT);
  pinMode(powerLEDPin, OUTPUT);

  digitalWrite(powerLEDPin, HIGH);  // Power LED always on

  Serial.begin(9600);
  Serial.println("CW KEYER by Ekin Efe GUNGOR");

  initLCD();
  updateLCDMode(decoderMode);
}

// --- Main Loop ---
void loop() {
  unsigned long now = millis();

  // --- Update WPM ---
  int potValue = analogRead(wpmPotPin);
  WPM = map(potValue, 0, 1023, 5, 35);
  dotDuration = 1200 / WPM;

  static int lastWPM = -1;
  if (WPM != lastWPM) {
    updateLCDMode(decoderMode);
    lastWPM = WPM;
  }

  // --- Mode toggle ---
  if (digitalRead(functionButton) == LOW && now - lastModeSwitch > 500) {
    decoderMode = (decoderMode + 1) % 3;
    updateLCDMode(decoderMode);
    lastModeSwitch = now;
  }

  // --- Paddle Logic ---
  bool dotPressed = digitalRead(dotPin) == LOW;
  bool dashPressed = digitalRead(dashPin) == LOW;

  if (keyingState == IDLE) {
    if (dotPressed && !dashPressed) {
      queuedElement = '.';
      keyingState = SENDING_DOT;
      keyingStart = now;
      playDot();
      morseBuffer += ".";
      lastKeyTime = now;
      justDecoded = false;
    } else if (dashPressed && !dotPressed) {
      queuedElement = '-';
      keyingState = SENDING_DASH;
      keyingStart = now;
      playDash();
      morseBuffer += "-";
      lastKeyTime = now;
      justDecoded = false;
    } else if (dotPressed && dashPressed) {
      queuedElement = lastWasDot ? '-' : '.';
      keyingState = (queuedElement == '.') ? SENDING_DOT : SENDING_DASH;
      keyingStart = now;
      if (queuedElement == '.') playDot();
      else playDash();
      morseBuffer += queuedElement;
      lastWasDot = !lastWasDot;
      lastKeyTime = now;
      justDecoded = false;
    }
  }

  if (keyingState != IDLE && (now - keyingStart >= keyingDuration)) {
    keyingState = POST_DELAY;
    keyingStart = now;
    keyingDuration = dotDuration;
  } else if (keyingState == POST_DELAY && (now - keyingStart >= keyingDuration)) {
    keyingState = IDLE;
  }

  // --- Decode Logic ---
  if (morseBuffer.length() > 0 && !dotPressed && !dashPressed &&
      (now - lastKeyTime >= dotDuration * 3) && !justDecoded) {
    decodeMorse(morseBuffer);
    morseBuffer = "";
    justDecoded = true;
    lastDecodeTime = now;
    Serial.println(sentenceBuffer);
    updateLCDText(sentenceBuffer);
  }

  // --- Add space between words ---
  if (justDecoded && !dotPressed && !dashPressed &&
      (now - lastDecodeTime >= dotDuration * 7)) {
    sentenceBuffer += " ";
    justDecoded = false;
    updateLCDText(sentenceBuffer);
  }

  // --- Clear button logic ---
  if (digitalRead(clearButton) == LOW) {
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
