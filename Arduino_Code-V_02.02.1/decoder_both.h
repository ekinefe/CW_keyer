#ifndef DECODER_BOTH_H
#define DECODER_BOTH_H

#include "PinSettings.h"
#include "Globals.h"
#include <Arduino.h>

// ----------------------------------------------------------------------------
// Decode a single Morse-code string into its corresponding alphanumeric
// character, update the sentenceBuffer according to decoderMode, and return it.
// Modes:
//   0 = append only the decoded character
//   1 = append only the raw Morse code (for display/debugging)
//   2 = append both code and character together
// ----------------------------------------------------------------------------

char decodeMorse(const String& code) {
  struct MorseMap {
    const char* code;
    char letter;
  };

  MorseMap morseTable[] = {
    {".-", 'A'}, {"-...", 'B'}, {"-.-.", 'C'}, {"-..", 'D'}, {".", 'E'},
    {"..-.", 'F'}, {"--.", 'G'}, {"....", 'H'}, {"..", 'I'}, {".---", 'J'},
    {"-.-", 'K'}, {".-..", 'L'}, {"--", 'M'}, {"-.", 'N'}, {"---", 'O'},
    {".--.", 'P'}, {"--.-", 'Q'}, {".-.", 'R'}, {"...", 'S'}, {"-", 'T'},
    {"..-", 'U'}, {"...-", 'V'}, {".--", 'W'}, {"-..-", 'X'}, {"-.--", 'Y'},
    {"--..", 'Z'}, {"-----", '0'}, {".----", '1'}, {"..---", '2'},
    {"...--", '3'}, {"....-", '4'}, {".....", '5'}, {"-....", '6'},
    {"--...", '7'}, {"---..", '8'}, {"----.", '9'}
  };

  for (auto &entry : morseTable) {
    if (code.equals(entry.code)) {
      if (decoderMode == 0) {
        sentenceBuffer += entry.letter;
      } else if (decoderMode == 1) {
        sentenceBuffer += code + " ";
      } else if (decoderMode == 2) {
        sentenceBuffer += code + entry.letter;
      }
      return entry.letter;
    }
  }

  // Not found
  sentenceBuffer += "? ";
  return '?';
}

#endif