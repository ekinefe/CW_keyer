#ifndef GLOBALS_H
#define GLOBALS_H

// ----------------------------------------------------------------------------
// Global variable declarations shared across modules:
//  - Decoder settings and buffers
//  - Encoder state machine and timing
//  - Morse audio parameters and lookup table
// ----------------------------------------------------------------------------

//=============================================
// --- Encoder state machine and LCD display ---
//=============================================
// Defines the states for the Serial-to-Morse encoding state machine
enum EncodeState {
    IDLE_ENCODE,    // Waiting for new text to encode
    ELEMENT_PLAY,   // Playing a dot or dash
    INTER_ELEMENT,  // Gap between elements in a character
    INTER_LETTER,   // Gap between letters
    WORD_GAP        // Gap between words
};
extern EncodeState encodeState = IDLE_ENCODE;   // Tracks the current state of the encoder (starts idle)
extern String encodeBuffer = "";                // Holds the full text string to be encoded to Morse
extern int charIndex = 0;                       // Index of the current character in encodeBuffer
extern int symbolIndex = 0;                     // Index of the current dot/dash symbol in currentMorse
extern unsigned long encodeStartTime = 0;       // Timestamp (millis) when the current encode state began
extern String currentMorse = "";                // Dot/dash string for the character currently being sent
extern int WPM;                                 // Words-per-minute speed setting (used below to compute timing)
extern int dotDuration = 1200 / WPM;            // Duration of one “dot” unit in milliseconds (1200 ms per word ÷ WPM)

//=============================================
// --- Decoder variables for paddle input ---
//=============================================
extern String morseBuffer = "";      // Accumulates dots and dashes entered via paddles
extern String sentenceBuffer = "";   // Stores decoded characters or raw code for display
extern bool lastWasDot = false;      // Used for iambic paddle alternation (true if last was a dot)
extern bool justDecoded = false;     // Flag indicating a complete Morse character was just decoded
extern int decoderMode = 0;          // Display mode: 0 = show decoded text only; 1 = show Morse only; 2 = show both

//=============================================
// --- Paddle state machine ---
//=============================================
extern int keyingLength = 0;          // Length (ms) of the current dot or dash during playback

//=============================================
// --- Morse audio parameters ---
//=============================================
extern int toneHz = 500;        // Frequency (Hz) for the Morse output tone
extern int newToneHz = 0;
extern int lastToneHz = -1;
extern int minHz = 400;
extern int maxHz = 1500;
extern int stepHz = 25;
extern int numSteps = (maxHz - minHz) / stepHz;  // =110
extern int WPM = 20;            // Default Words-Per-Minute value for encoding/decoding timing
extern int lastWPM = -1;

//=============================================
// --- CLI Debuging/LOGing ---
//=============================================
extern String DefaultLocation = "Default Location";
extern String DefaultMessage = "Default Message";

//=============================================
// --- Morse code lookup table for A–Z and 0–9 ---
//=============================================
// Indices 0–25 map to A–Z; indices 26–35 map to digits 0–9.
// Each entry is a null-terminated string of '.' and '-' characters.
extern const char* morseTable[36] = {
  ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..",    // A-I
  ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.",  // J-R
  "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..",         // S-Z
  
  "-----", ".----", "..---", "...--", "....-", ".....", "-....",    // 0-5
  "--...", "---..", "----."                                        // 6-9
};

#endif