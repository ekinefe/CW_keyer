#ifndef DECODER_BOTH_H
#define DECODER_BOTH_H

#include <Arduino.h>

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
      Serial.print(code);
      Serial.print(" → ");
      Serial.println(entry.letter);
      return entry.letter;
    }
  }
  Serial.print(code);
  Serial.println(" → ?");
  return '?';
}

#endif
