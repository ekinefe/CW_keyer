#ifndef DECODER_MORSE_ONLY_H
#define DECODER_MORSE_ONLY_H

#include <Arduino.h>

char decodeMorse(const String& code) {
  Serial.println(code);
  return '?';  // No actual decoding needed
}

#endif
