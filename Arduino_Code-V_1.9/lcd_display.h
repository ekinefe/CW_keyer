#ifndef LCD_DISPLAY_H
#define LCD_DISPLAY_H

#include <LiquidCrystal.h>
extern int WPM;


// LCD pins (RS, E, D4, D5, D6, D7)
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// --- Play startup sound (must be declared before use) ---
void playStartupSound() {
  const int toneHz = 600;
  const int unit = 52;

  // CQ: -.-. --.-
  // C: -.-.
  tone(8, toneHz); delay(unit * 3); noTone(8); delay(unit); // Dah
  tone(8, toneHz); delay(unit);     noTone(8); delay(unit); // Dit
  tone(8, toneHz); delay(unit * 3); noTone(8); delay(unit); // Dah
  tone(8, toneHz); delay(unit);     noTone(8); delay(unit); // Dit

  // Q: --.-
  tone(8, toneHz); delay(unit * 3); noTone(8); delay(unit); // Dah
  tone(8, toneHz); delay(unit * 3); noTone(8); delay(unit); // Dah
  tone(8, toneHz); delay(unit);     noTone(8); delay(unit); // Dit
  tone(8, toneHz); delay(unit * 3); noTone(8);              // Dah
}


void initLCD() {
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("CW Keyer Ready");

  playStartupSound();  // now it's declared!
  delay(2000);
  lcd.clear();
}

void updateLCDMode(int mode) {
  lcd.setCursor(0, 0);
  lcd.print("MODE: ");
  if (mode == 0) lcd.print("CHAR ");
  else if (mode == 1) lcd.print("MORSE");
  else lcd.print("BOTH ");

  // if (WPM >= )
  byte specialChar[8] = {
    B10101,
    B01010,
    B00000,
    B00111,
    B00111,
    B00100,
    B01010,
    B10101
  };

  lcd.createChar(0, specialChar);  // Load to slot 0
  lcd.setCursor(13, 0);        // Row 1, column 0
  lcd.write(byte(0)); 

  lcd.setCursor(14, 0);
  lcd.print(WPM);
}

void updateLCDText(const String& sentenceBuffer) {
  lcd.setCursor(0, 1);

  String text = sentenceBuffer;
  if (text.length() > 12) text = text.substring(text.length() - 12);

  lcd.print(text);
  int rem = 12 - text.length();
  for (int i = 0; i < rem; i++) lcd.print(" ");
}

void lcdPrintFinal(const String& text) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("FINAL:");
  lcd.setCursor(0, 1);
  if (text.length() <= 16) {
    lcd.print(text);
  } else {
    lcd.print(text.substring(text.length() - 16));
  }
}


#endif
