#include <LiquidCrystal.h>
#include "lcd_display.h"
#include "decoder_both.h"  // You can switch to decoder_char_only.h or decoder_morse_only.h

// --- Pin Configuration ---
const int dotPin = A0;               // Dot paddle pin
const int dashPin = A1;              // Dash paddle pin
const int buzzerPin = 8;             // Buzzer output pin
const int morseLEDPin = 10;          // LED synchronized with Morse output
const int functionButtonPin = 7;     // Button to switch decode modes
const int clearButtonPin = 6;        // Button to clear display
const int wpmPotPin = A2;            // Potentiometer for WPM control

// --- Morse Timing Settings ---
int WPM = 20;                        // Initial Words Per Minute
int dotDuration = 1200 / WPM;        // Dot duration (ms), calculated as 1200 / WPM
const int toneHz = 600;              // Buzzer tone frequency (Hz)

// --- State Buffers ---
String morseBuffer = "";             // Stores sequence of dots and dashes for decoding
String sentenceBuffer = "";          // Stores final decoded characters
bool lastWasDot = false;             // Used when both paddles are pressed

// --- Time Tracking ---
unsigned long lastKeyTime = 0;
unsigned long lastDecodeTime = 0;
unsigned long lastModeSwitchTime = 0;
bool justDecoded = false;
int decoderMode = 0;                 // 0=CHAR, 1=MORSE, 2=BOTH

// --- Keying State Machine ---
enum KeyingState { IDLE, SENDING_DOT, SENDING_DASH, POST_DELAY };
KeyingState keyingState = IDLE;
unsigned long keyingStartTime = 0;
int keyingLength = 0;
char queuedElement = '\0';

// --- Setup: Pin init, Serial, LCD ---
void setup() {
  pinMode(dotPin, INPUT_PULLUP);
  pinMode(dashPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
  pinMode(morseLEDPin, OUTPUT);
  pinMode(functionButtonPin, INPUT_PULLUP);
  pinMode(clearButtonPin, INPUT_PULLUP);

  Serial.begin(9600);
  Serial.println("CW KEYER by Ekin Efe GUNGOR");

  initLCD();  // LCD + startup tone animation
  updateLCDMode(decoderMode); 
  updateWPMLevel(WPM);
}

// --- Main Loop ---
void loop() {
  unsigned long now = millis();

  // --- Read Pot and Update WPM ---
  int potValue = analogRead(wpmPotPin);
  WPM = map(potValue, 0, 1023, 5, 35);
  dotDuration = 1200 / WPM;

  // Update WPM on LCD if changed
  static int lastWPM = -1;
  if (WPM != lastWPM) {
    updateLCDMode(decoderMode);
    updateWPMLevel(WPM);
    lastWPM = WPM;
  }

  // --- Toggle Decoder Mode Button ---
  if (digitalRead(functionButtonPin) == LOW && now - lastModeSwitchTime > 500) {
    decoderMode = (decoderMode + 1) % 3;
    updateLCDMode(decoderMode);
    lastModeSwitchTime = now;
  }

  // --- Paddle Input Handling ---
  bool dotPressed = digitalRead(dotPin) == LOW;
  bool dashPressed = digitalRead(dashPin) == LOW;

  // --- State Machine for Dot/Dash Sending ---
  if (keyingState == IDLE) {
    if (dotPressed && !dashPressed) {
      queuedElement = '.';
      keyingLength = dotDuration;
      playDot();  // Play sound + LED
      morseBuffer += ".";
      startKeying(now);
    }
    else if (dashPressed && !dotPressed) {
      queuedElement = '-';
      keyingLength = dotDuration * 3;
      playDash();  // Play sound + LED
      morseBuffer += "-";
      startKeying(now);
    }
    else if (dotPressed && dashPressed) {
      queuedElement = lastWasDot ? '-' : '.';
      keyingLength = (queuedElement == '.') ? dotDuration : dotDuration * 3;
      if (queuedElement == '.') playDot(); else playDash();
      morseBuffer += queuedElement;
      startKeying(now);
      lastWasDot = !lastWasDot;
    }
  }
  else if (keyingState == SENDING_DOT || keyingState == SENDING_DASH) {
    if (now - keyingStartTime >= keyingLength) {
      stopTone();  // Turn off sound + LED
      keyingStartTime = now;
      keyingState = POST_DELAY;
      keyingLength = dotDuration;
    }
  }
  else if (keyingState == POST_DELAY && now - keyingStartTime >= keyingLength) {
    keyingState = IDLE;
  }

  // --- Decode After 3 Dot-Length Pause ---
  if (morseBuffer.length() > 0 && !dotPressed && !dashPressed &&
      now - lastKeyTime >= dotDuration * 3 && !justDecoded) {
    decodeMorse(morseBuffer);
    morseBuffer = "";
    justDecoded = true;
    lastDecodeTime = now;
    Serial.println(sentenceBuffer);
    updateLCDText(sentenceBuffer);
  }

  // --- Word Gap Insert ---
  if (justDecoded && !dotPressed && !dashPressed &&
      now - lastDecodeTime >= dotDuration * 7) {
    sentenceBuffer += " ";
    justDecoded = false;
    updateLCDText(sentenceBuffer);
  }

  // --- Clear Button Handling ---
  if (digitalRead(clearButtonPin) == LOW) {
    sentenceBuffer = "";
    morseBuffer = "";
    justDecoded = false;
    lcd.clear();
    updateLCDMode(decoderMode);
    updateWPMLevel(WPM);
    updateLCDText(sentenceBuffer);
    Serial.println("=== CLEARED ===");
    delay(300);  // debounce
  }
}

// --- Start sending state ---
void startKeying(unsigned long now) {
  keyingState = (queuedElement == '.') ? SENDING_DOT : SENDING_DASH;
  keyingStartTime = now;
  lastKeyTime = now;
  justDecoded = false;
}

// --- Turn off buzzer and LED ---
void stopTone() {
  noTone(buzzerPin);
  digitalWrite(morseLEDPin, LOW);
}

// --- Play a dot: tone and LED ---
void playDot() {
  tone(buzzerPin, toneHz);
  digitalWrite(morseLEDPin, HIGH);
}

// --- Play a dash: tone and LED ---
void playDash() {
  tone(buzzerPin, toneHz);
  digitalWrite(morseLEDPin, HIGH);
}
