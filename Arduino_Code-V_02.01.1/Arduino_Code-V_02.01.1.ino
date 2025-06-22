#include "lcd_display.h"
#include "decoder_both.h"
#include "PinSettings.h"
#include "Globals.h"
#include "Encoder.h"
#include "Button_Controoller.h"
#include "Default_Sounds.h"
#include "Buffer_Test.h"
#include "WPM_Controller.h"
#include "Buffer_Cleanner.h"



// Timing helpers
unsigned long lastKeyTime = 0;
unsigned long lastDecodeTime = 0;
unsigned long lastModeSwitchTime = 0;

// Paddle state machine
enum KeyingState { IDLE, SENDING_DOT, SENDING_DASH, POST_DELAY };
KeyingState keyingState = IDLE;
unsigned long keyingStartTime = 0;

char queuedElement = '\0';

// ----------------------------
//            SETUP
// ----------------------------
void setup() {
  pinMode(dotPin, INPUT_PULLUP);
  pinMode(dashPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
  pinMode(morseLEDPin, OUTPUT);
  pinMode(powerLEDPin, OUTPUT);
  pinMode(functionButtonPin, INPUT_PULLUP);
  pinMode(clearButtonPin, INPUT_PULLUP);

  digitalWrite(powerLEDPin, HIGH);

  Serial.begin(9600);
  Serial.println("");
  Serial.println("CW KEYER by Ekin Efe GUNGOR");

  Serial.print("Opening Sound - ");
  initLCD();  //Opening Print on LCD
  CQ();       //Opening Sound
  updateLCDText(sentenceBuffer);
  updateLCDMode(decoderMode);
  updateWPMLevel(WPM);
  // CleanBuffer();
  
  // static bool done = false;
  // if (!done) {
  //   BufferTest tester;
  //   tester.fillAndPlay();
  //   Serial.println(tester.getBuffer());
  //   done = true;
  // }
}

// ----------------------------
//           LOOP
// ----------------------------
void loop() {
  unsigned long now = millis();

  // --- Update WPM from Potentiometer ---
  WPM_Controller();

  // --- Switch Decoder Mode ---
  if (digitalRead(functionButtonPin) == LOW && now - lastModeSwitchTime > 500) {
    decoderMode = (decoderMode + 1) % 3;
    updateLCDMode(decoderMode);
    lastModeSwitchTime = now;
  }

  // --- Paddle Input State Machine ---
  bool dotPressed = digitalRead(dotPin) == LOW;
  bool dashPressed = digitalRead(dashPin) == LOW;

  if (keyingState == IDLE) {
    if (dotPressed && !dashPressed) {
      queuedElement = '.';
      keyingLength = dotDuration;
      playDot();
      morseBuffer += ".";
      startKeying(now);
    } else if (dashPressed && !dotPressed) {
      queuedElement = '-';
      keyingLength = dotDuration * 3;
      playDash();
      morseBuffer += "-";
      startKeying(now);
    } else if (dotPressed && dashPressed) {
      queuedElement = lastWasDot ? '-' : '.';
      keyingLength = (queuedElement == '.') ? dotDuration : dotDuration * 3;
      if (queuedElement == '.') playDot(); else playDash();
      morseBuffer += queuedElement;
      startKeying(now);
      lastWasDot = !lastWasDot;
    }
  } else if (keyingState == SENDING_DOT || keyingState == SENDING_DASH) {
    if (now - keyingStartTime >= keyingLength) {
      stopTone();
      keyingStartTime = now;
      keyingState = POST_DELAY;
      keyingLength = dotDuration;
    }
  } else if (keyingState == POST_DELAY && now - keyingStartTime >= keyingLength) {
    keyingState = IDLE;
  }

  // --- Decode Morse ---
  if (morseBuffer.length() > 0 && !dotPressed && !dashPressed &&
      now - lastKeyTime >= dotDuration * 3 && !justDecoded) {
    decodeMorse(morseBuffer);
    morseBuffer = "";
    justDecoded = true;
    lastDecodeTime = now;
    Serial.println(sentenceBuffer);
    updateLCDText(sentenceBuffer);
  }

  // --- Word Gap Handling ---
  if (justDecoded && !dotPressed && !dashPressed &&
      now - lastDecodeTime >= dotDuration * 7) {
    sentenceBuffer += " ";
    justDecoded = false;
    updateLCDText(sentenceBuffer);
  }

  // --- Clear Button ---
  if (digitalRead(clearButtonPin) == LOW) {
    clearButton();
  }

  // --- Run Serial Encoder ---
  handleEncoding(millis());
}

// ----------------------------
//      FUNCTION DEFINITIONS
// ----------------------------

void startKeying(unsigned long now) {
  keyingState = (queuedElement == '.') ? SENDING_DOT : SENDING_DASH;
  keyingStartTime = now;
  lastKeyTime = now;
  justDecoded = false;
}