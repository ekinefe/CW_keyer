// --- Select your decoder mode ---
#include "decoder_char_only.h"
// #include "decoder_morse_only.h"
// #include "decoder_both.h"  // <== Change this to select output mode

const int WPM = 20;
const int TONE_HZ = 600;

const int ditPin = 2;
const int dahPin = 3;
const int buzzerPin = 9;

unsigned int ditDuration;
bool lastWasDit = false;
String morseBuffer = "";

unsigned long lastKeyTime = 0;
unsigned long lastDecodeTime = 0;
bool justDecoded = false;

void setup() {
  pinMode(ditPin, INPUT_PULLUP);
  pinMode(dahPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
  Serial.begin(9600);
  ditDuration = 1200 / WPM;

  Serial.println("CW Keyer with Accurate Spacing Ready");
}

void loop() {
  bool ditPressed = digitalRead(ditPin) == LOW;
  bool dahPressed = digitalRead(dahPin) == LOW;
  unsigned long now = millis();

  // --- Key input ---
  if (ditPressed && !dahPressed) {
    sendDit();
    morseBuffer += ".";
    lastKeyTime = now;
    justDecoded = false;
  } else if (dahPressed && !ditPressed) {
    sendDah();
    morseBuffer += "-";
    lastKeyTime = now;
    justDecoded = false;
  } else if (ditPressed && dahPressed) {
    if (lastWasDit) {
      sendDah();
      morseBuffer += "-";
    } else {
      sendDit();
      morseBuffer += ".";
    }
    lastWasDit = !lastWasDit;
    lastKeyTime = now;
    justDecoded = false;
  }

  // --- Character gap detection (≥ 3 dits of silence) ---
  if (morseBuffer.length() > 0 && !ditPressed && !dahPressed && (now - lastKeyTime >= ditDuration * 3) && !justDecoded) {
    decodeMorse(morseBuffer);
    morseBuffer = "";
    lastDecodeTime = now;
    justDecoded = true;
  }

  // --- Word gap detection (≥ 7 dits after character decoded) ---
  if (justDecoded && !ditPressed && !dahPressed && (now - lastDecodeTime >= ditDuration * 7)) {
    Serial.print(" ");
    justDecoded = false;  // prevent duplicate spaces
  }

  delay(5);
}

void sendDit() {
  tone(buzzerPin, TONE_HZ);
  delay(ditDuration);
  noTone(buzzerPin);
  delay(ditDuration); // inter-element
}

void sendDah() {
  tone(buzzerPin, TONE_HZ);
  delay(ditDuration * 3);
  noTone(buzzerPin);
  delay(ditDuration); // inter-element
}
