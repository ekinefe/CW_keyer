#ifndef GLOBALS_H
#define GLOBALS_H

// Declare external variables --- Decoder Both
// extern int decoderMode;
// extern String sentenceBuffer;

// Serial encoder state machine --- LCD Display
enum EncodeState { IDLE_ENCODE, ELEMENT_PLAY, INTER_ELEMENT, INTER_LETTER, WORD_GAP };
extern EncodeState encodeState = IDLE_ENCODE;
extern String encodeBuffer = "";
extern int charIndex = 0;
extern int symbolIndex = 0;
extern unsigned long encodeStartTime = 0;
extern String currentMorse = "";
extern int WPM;
extern int dotDuration = 1200 / WPM;  // Calculated from WPM

extern String morseBuffer = "";      // Accumulate morse from paddles
extern String sentenceBuffer = "";   // Final decoded sentence
extern bool lastWasDot = false;      // For iambic paddle alternation
extern bool justDecoded = false;     // Used to manage decoding flow
extern int decoderMode = 0;          // 0=CHAR, 1=MORSE, 2=BOTH

// Paddle state machine
extern int keyingLength = 0;

// --- Morse Audio Parameters ---
extern const int toneHz = 500;        // Morse tone frequency
extern int WPM = 20;                  // Words Per Minute

// --- Morse Table for A-Z and 0-9 ---
extern const char* morseTable[36] = {
  ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..",    // A-I
  ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.",  // J-R
  "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..",         // S-Z
  
  "-----", ".----", "..---", "...--", "....-", ".....", "-....",    // 0-5
  "--...", "---..", "----."                                        // 6-9
};

#endif