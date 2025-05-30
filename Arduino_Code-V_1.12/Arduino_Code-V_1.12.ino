#include <LiquidCrystal.h>
#include "lcd_display.h"
#include "decoder_both.h"  // You can replace this with your preferred decoder header

// ----------------------------
//          CONSTANTS
// ----------------------------

// --- Pin Assignments ---
const int dotPin = A0;
const int dashPin = A1;
const int buzzerPin = 8;
const int morseLEDPin = 10;
const int powerLEDPin = 9;
const int functionButtonPin = 7;
const int clearButtonPin = 6;
const int wpmPotPin = A2;

// --- Morse Table for A-Z and 0-9 ---
const char* morseTable[36] = {
  ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..",    // A-I
  ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.",  // J-R
  "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..",         // S-Z
  "-----", ".----", "..---", "...--", "....-", ".....", "-....",    // 0-5
  "--...", "---..", "----."                                        // 6-9
};

// --- Morse Audio Parameters ---
const int toneHz = 550;        // Morse tone frequency
int WPM = 20;                  // Words Per Minute
int dotDuration = 1200 / WPM;  // Calculated from WPM

// ----------------------------
//        BUFFERS / STATE
// ----------------------------

String morseBuffer = "";      // Accumulate morse from paddles
String sentenceBuffer = "";   // Final decoded sentence
bool lastWasDot = false;      // For iambic paddle alternation
bool justDecoded = false;     // Used to manage decoding flow
int decoderMode = 0;          // 0=CHAR, 1=MORSE, 2=BOTH

// Timing helpers
unsigned long lastKeyTime = 0;
unsigned long lastDecodeTime = 0;
unsigned long lastModeSwitchTime = 0;

// Paddle state machine
enum KeyingState { IDLE, SENDING_DOT, SENDING_DASH, POST_DELAY };
KeyingState keyingState = IDLE;
unsigned long keyingStartTime = 0;
int keyingLength = 0;
char queuedElement = '\0';

// Serial encoder state machine
enum EncodeState { IDLE_ENCODE, ELEMENT_PLAY, INTER_ELEMENT, INTER_LETTER, WORD_GAP };
EncodeState encodeState = IDLE_ENCODE;
String encodeBuffer = "";
int charIndex = 0;
int symbolIndex = 0;
String currentMorse = "";
unsigned long encodeStartTime = 0;

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

  initLCD();
  updateLCDMode(decoderMode);
  updateWPMLevel(WPM);
}

// ----------------------------
//           LOOP
// ----------------------------
void loop() {
  unsigned long now = millis();

  // --- Update WPM from Potentiometer ---
  int potValue = analogRead(wpmPotPin);
  WPM = map(potValue, 0, 1023, 5, 35);
  dotDuration = 1200 / WPM;
  static int lastWPM = -1;
  if (WPM != lastWPM) {
    updateLCDMode(decoderMode);
    updateWPMLevel(WPM);
    lastWPM = WPM;
  }

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

void playDot() {
  tone(buzzerPin, toneHz);
  digitalWrite(morseLEDPin, HIGH);
}

void playDash() {
  tone(buzzerPin, toneHz);
  digitalWrite(morseLEDPin, HIGH);
}

void stopTone() {
  noTone(buzzerPin);
  digitalWrite(morseLEDPin, LOW);
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
