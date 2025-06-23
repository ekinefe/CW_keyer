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
#include "Hz_Controller.h"
#include "CLI_Debuger.h"


// ----------------------------------------------------------------------------
// Timing helpers
// ----------------------------------------------------------------------------
unsigned long lastKeyTime         = 0;    // When last paddle element began
unsigned long lastDecodeTime      = 0;    // When last character was decoded
unsigned long lastModeSwitchTime  = 0;    // Debounce timer for mode button

// ----------------------------------------------------------------------------
// Paddle keying state machine
// ----------------------------------------------------------------------------
enum KeyingState { IDLE, SENDING_DOT, SENDING_DASH, POST_DELAY };
KeyingState   keyingState       = IDLE;   // Current manual-keying phase
unsigned long keyingStartTime   = 0;      // Timestamp for start of dot/dash or gap
char          queuedElement     = '\0';   // Next symbol to send: '.' or '-'

// ----------------------------
//           SETUP
// ----------------------------
void setup() {
  // Configure I/O pin modes
  pinMode(dotPin, INPUT_PULLUP);            // Dot paddle (active LOW)
  pinMode(dashPin, INPUT_PULLUP);           // Dash paddle
  pinMode(buzzerPin, OUTPUT);               // Buzzer output
  pinMode(morseLEDPin, OUTPUT);             // Morse-LED indicator
  pinMode(powerLEDPin, OUTPUT);             // Power-status LED
  pinMode(functionButtonPin, INPUT_PULLUP); // Mode toggle button
  pinMode(clearButtonPin, INPUT_PULLUP);    // Clear buffer button

  // Power LED ON
  digitalWrite(powerLEDPin, HIGH);

  // Initialize Serial for debug and Serial-to-Morse
  Serial.begin(9600);
  Serial.println("");                    
  Serial.println("CW KEYER by Ekin Efe GUNGOR"); // Startup banner

  // Play opening sequence and initialize LCD
  Serial.print("Opening Sound - ");
  initLCD();                             // Show welcome message
  CQ();                                  // Emit “CQ” tone
  updateLCDText(sentenceBuffer);         // Display (empty) buffer
  updateLCDMode(decoderMode);            // Display current mode icon
  updateWPMLevel(WPM);                   // Display current speed
  // CleanBuffer();                      // Optional: clear buffers/display

  // Legacy buffer-test code (disabled):
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

  
  // --- Switch Decoder Mode ---
  if (digitalRead(functionButtonPin) == LOW && now - lastModeSwitchTime > 500) {
    decoderMode = (decoderMode + 1) % 3;
    updateLCDMode(decoderMode);
    lastModeSwitchTime = now;
  }

  // --- Clear Button ---
  if (digitalRead(clearButtonPin) == LOW) {
    clearButton();
  }

  // --- Update WPM from Potentiometer ---
  // if (WPM != lastWPM){
    WPM_Controller();
  // }

  // --- Update toneHz from Potentiometer ---
  if (CheckHertz()){
    hzController();
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