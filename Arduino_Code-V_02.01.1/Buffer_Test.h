#ifndef BUFFER_TEST_H
#define BUFFER_TEST_H

#include <Arduino.h>
#include "PinSettings.h"
#include "Globals.h"    // extern String sentenceBuffer;
#include "Encoder.h"    // encodeBuffer, charIndex, symbolIndex, encodeState, encodeStartTime, handleEncoding
#include "lcd_display.h" // updateLCDText, lcdPrintFinal

class BufferTest {
public:
  static const uint8_t MAX_LEN = 100;

  BufferTest() {
    randomSeed(analogRead(A0));
    sentenceBuffer = "";                  // global buffer'ı temizle
    updateLCDText(sentenceBuffer);         // LCD'yi temizle ve başlat
    Serial.println("[BufferTest] Başlatıldı");
  }

  // Bir rastgele karakter ekle, anında çal, ve hem seri hem LCD'ye yaz
  bool addAndPlayRandom() {
    if (sentenceBuffer.length() >= MAX_LEN) return false;

    char c = randomChar();
    sentenceBuffer += c;                   // global buffer'a ekle

    // Konsol ve LCD güncelle
    Serial.print(c);
    updateLCDText(sentenceBuffer);

    // Morse encode akışını başlat
    encodeBuffer    = String(c);
    charIndex       = 0;
    symbolIndex     = 0;
    encodeState     = INTER_LETTER;
    encodeStartTime = millis();
    while (encodeState != IDLE_ENCODE) {
      handleEncoding(millis());
    }

    delay(10);  // karakterler arası küçük boşluk
    return true;
  }

  // Buffer dolana kadar ekleme ve oynatma
  void fillAndPlay() {
    while (addAndPlayRandom()) {
      // kendi içinde çalışıyor
    }
    Serial.println();
    Serial.print("[BufferTest] Tamamlandı: ");
    Serial.println(sentenceBuffer);

    // Final metni büyük puntoda göster
    lcd.clear();
    lcdPrintFinal(sentenceBuffer);
  }

  // Buffer içeriğini döner
  String getBuffer() const {
    return sentenceBuffer;
  }

private:
  // Rastgele karakter havuzu
  char randomChar() {
    const char* pool = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 ";
    size_t idx = random(strlen(pool));
    return pool[idx];
  }
};

#endif // BUFFER_TEST_H
