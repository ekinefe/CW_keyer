#ifndef CLI_DISPLAY_H
#define CLI_DISPLAY_H

#include <Arduino.h>

extern String sentenceBuffer;

void updateCLIDisplay(const String& sentenceBuffer) {
  static String lastPrinted = "";

  if (sentenceBuffer != lastPrinted) {
    Serial.print("Decoded: ");
    Serial.println(sentenceBuffer);
    lastPrinted = sentenceBuffer;
  }
}

void printToCLI(const String& sentenceBuffer) {
  updateCLIDisplay(sentenceBuffer);
}


void printFinalToCLI() {
  Serial.println("Final Output:");
  Serial.println(sentenceBuffer);
}

#endif
