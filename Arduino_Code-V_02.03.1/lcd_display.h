#ifndef LCD_DISPLAY_H
#define LCD_DISPLAY_H

#include "PinSettings.h"
#include "Globals.h"
#include <LiquidCrystal.h>

// Instantiate the LCD object with pins: RS, E, D4, D5, D6, D7
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// ----------------------------------------------------------------------------
// initLCD()
//   - Configure the LCD for 16×2 characters.
//   - Show a startup message ("CW Keyer Ready") for 2 seconds.
//   - Clear the screen to prepare for normal operation.
// ----------------------------------------------------------------------------
void initLCD() {
  lcd.begin(16, 2);            // Initialize columns and rows
  lcd.clear();                 // Clear any previous display content
  lcd.setCursor(0, 0);         // Move cursor to first column, first row
  lcd.print("CW Keyer Ready"); // Show welcome text

  delay(2000);                 // Pause 2 seconds so user can read it
  lcd.clear();                 // Clear before entering normal loop
}

// ----------------------------------------------------------------------------
// updateLCDMode(mode)
//   - Display the current decoder mode on the top line.
//   - mode == 0 : show "CHAR"
//   - mode == 1 : show "MRS" (Morse)
//   - mode == 2 : show "BOTH"
// ----------------------------------------------------------------------------
void updateLCDMode(int mode) {
  lcd.setCursor(0, 0);     // Top-left corner
  lcd.print("MODE: ");     // Static label
  if (mode == 0) {         // Character-only mode
    lcd.print("CHAR");
  }
  
  else if (mode == 1) {    // Morse-only mode
    lcd.print("MRS");
  }
  
  else {                   // Both modes
    lcd.print("BOTH");
  }
}

// ----------------------------------------------------------------------------
// updateWPMLevel(WPM)
//   - Draws a bar-graph icon representing speed:
//       level1 (slowest) ... level8 (fastest).
//   - Uses lcd.createChar() to program a custom 5×8 matrix.
//   - Prints the numeric WPM value, right-aligned.
// ----------------------------------------------------------------------------
void updateWPMLevel(int WPM) {
  // Define 8 custom characters for bar-graph levels (rows of 5 pixels):
  byte level1[8] = {
    B00000, B00000, B00000, B00000, B00000, B00000, B00000, B11111
  };
  byte level2[8] = {
    B00000, B00000, B00000, B00000, B00000, B00000, B11111, B11111
  };
  byte level3[8] = {
    B00000, B00000, B00000, B00000, B00000, B11111, B11111, B11111
  };
  byte level4[8] = {
    B00000, B00000, B00000, B00000, B11111, B11111, B11111, B11111
  };
  byte level5[8] = {
    B00000, B00000, B00000, B11111, B11111, B11111, B11111, B11111
  };
  byte level6[8] = {
    B00000, B00000, B11111, B11111, B11111, B11111, B11111, B11111
  };
  byte level7[8] = {
    B00000, B11111, B11111, B11111, B11111, B11111, B11111, B11111
  };
  byte level8[8] = {
    B11111, B11111, B11111, B11111, B11111, B11111, B11111, B11111
  };

  // Choose which bar level to show based on WPM thresholds:
  if (WPM < 8) {                  // Level 1 for very slow speeds
    lcd.createChar(0, level1);    // Store in custom-char slot 0
    lcd.setCursor(13, 0);         // Place near right end of top line
    lcd.write(byte(0));           // Draw the bar graph
  } 
  else if (WPM < 16) {            // Level 2
    lcd.createChar(0, level2);
    lcd.setCursor(13, 0);
    lcd.write(byte(0));
  } 
  else if (WPM < 24) {            // Level 3
    lcd.createChar(0, level3);
    lcd.setCursor(13, 0);
    lcd.write(byte(0));
  } 
  else if (WPM < 28) {            // Level 4
    lcd.createChar(0, level4);
    lcd.setCursor(13, 0);
    lcd.write(byte(0));
  } 
  else if (WPM < 30) {            // Level 5
    lcd.createChar(0, level5);  
    lcd.setCursor(13, 0);
    lcd.write(byte(0));
  } 
  else if (WPM < 33) {            // Level 6
    lcd.createChar(0, level6);
    lcd.setCursor(13, 0);
    lcd.write(byte(0));
  } 
  else if (WPM < 33) {            // Level 7 (note: same threshold as level 6; legacy)
    lcd.createChar(0, level7);
    lcd.setCursor(13, 0);
    lcd.write(byte(0));
  } 
  else {                          // Level 8 for highest speeds
    lcd.createChar(0, level8);
    lcd.setCursor(13, 0);
    lcd.write(byte(0));
  }

  // Print the WPM number itself, right-aligned:
  if (WPM >= 5 && WPM < 10) {     // Single-digit WPM
    lcd.setCursor(15, 0);         // Last column
    lcd.print(WPM);
  } 
  else {                        // Two-digit or three-digit WPM
    lcd.setCursor(14, 0);         // Second-to-last column
    lcd.print(WPM);
  }
}

// ----------------------------------------------------------------------------
// updateLCDText(sentenceBuffer)
//   - Show the last up to 12 characters on the second line.
//   - If buffer is longer, truncate to the rightmost 12.
//   - Pads with spaces to clear leftover characters.
// ----------------------------------------------------------------------------
void updateLCDText(const String& sentenceBuffer) {
  lcd.setCursor(0, 1);                  // Move to start of second line
  String text = sentenceBuffer;         // Copy to allow truncation
  if (text.length() > 12)               // If too long, keep only last 12 chars
    text = text.substring(text.length() - 12);
  lcd.print(text);                      // Display truncated text
  int rem = 12 - text.length();         // Calculate how many spaces needed
  for (int i = 0; i < rem; i++)         // Clear any previous chars to the right
    lcd.print(" ");
}

// ----------------------------------------------------------------------------
// lcdPrintFinal(text)
//   - Clears the display and shows a “FINAL:” header on line 1.
//   - Prints the full or truncated (last 16) text on line 2.
// ----------------------------------------------------------------------------
void lcdPrintFinal(const String& text) {
  lcd.clear();                  // Wipe entire screen
  lcd.setCursor(0, 0);          // Top line
  lcd.print("FINAL:");          // Fixed header
  lcd.setCursor(0, 1);          // Begin second line
  if (text.length() <= 16) {    // If it fits, print directly
    lcd.print(text);
  }
  else {                        // Otherwise show last 16 chars
    lcd.print(text.substring(text.length() - 16));
  }
}

void updateLCDToneHz(int toneHz) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("TONE Hz:");
  lcd.setCursor(0, 1);
  lcd.print(toneHz);
  delay(1000);

  lcd.clear();
  updateLCDMode(decoderMode);
  updateWPMLevel(WPM);
  updateLCDText(sentenceBuffer);
}

void clearLCDText () {
  lcd.clear();
  updateLCDMode(decoderMode);
  updateWPMLevel(WPM);
  lcd.setCursor(0, 1);
  lcd.print("====CLEARED====");
  delay(500);
  lcd.clear();
  updateLCDMode(decoderMode);
  updateWPMLevel(WPM);
  updateLCDText(sentenceBuffer);
}


#endif