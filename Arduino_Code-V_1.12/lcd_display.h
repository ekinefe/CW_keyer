#ifndef LCD_DISPLAY_H
#define LCD_DISPLAY_H

#include <LiquidCrystal.h>
extern int WPM;


// LCD pins (RS, E, D4, D5, D6, D7)
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void initLCD() {
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("CW Keyer Ready");

  // playStartupSound();  // now it's declared!
  delay(2000);
  lcd.clear();
}

void updateLCDMode(int mode) {
  lcd.setCursor(0, 0);
  lcd.print("MODE: ");
  if (mode == 0) lcd.print("CHAR ");
  else if (mode == 1) lcd.print("MRS ");
  else lcd.print("BOTH ");
}
void updateWPMLevel(int WPM){
  byte level1[8] = {
      B00000,
      B00000,
      B00000,
      B00000,
      B00000,
      B00000,
      B00000,
      B11111
  };
  byte level2[8] = {
      B00000,
      B00000,
      B00000,
      B00000,
      B00000,
      B00000,
      B11111,
      B11111
  };
  byte level3[8] = {
      B00000,
      B00000,
      B00000,
      B00000,
      B00000,
      B11111,
      B11111,
      B11111
  };
  byte level4[8] = {
      B00000,
      B00000,
      B00000,
      B00000,
      B11111,
      B11111,
      B11111,
      B11111
  };
  byte level5[8] = {
      B00000,
      B00000,
      B00000,
      B11111,
      B11111,
      B11111,
      B11111,
      B11111
  };
  byte level6[8] = {
      B00000,
      B00000,
      B11111,
      B11111,
      B11111,
      B11111,
      B11111,
      B11111
  };
  byte level7[8] = {
      B00000,
      B11111,
      B11111,
      B11111,
      B11111,
      B11111,
      B11111,
      B11111
  };
  byte level8[8] = {
      B11111,
      B11111,
      B11111,
      B11111,
      B11111,
      B11111,
      B11111,
      B11111
  };
  
  if (WPM >= 5 && WPM <9){ // Print Level 1
    lcd.createChar(0, level1);  
    lcd.setCursor(13, 0);       
    lcd.write(byte(0)); 
  }
  else if (WPM >= 9 && WPM <13){  //Print Level 2
    lcd.createChar(0, level2);  
    lcd.setCursor(13, 0);       
    lcd.write(byte(0)); 
  }
  else if (WPM >= 13 && WPM <17){ //Print Level 3
    lcd.createChar(0, level3);  
    lcd.setCursor(13, 0);       
    lcd.write(byte(0)); 
  }
  else if (WPM >= 17 && WPM <21){ //print Level 4
    lcd.createChar(0, level4);  
    lcd.setCursor(13, 0);       
    lcd.write(byte(0)); 
  }
  else if (WPM >= 21 && WPM <25){ //print level 5
    lcd.createChar(0, level5);  
    lcd.setCursor(13, 0);       
    lcd.write(byte(0)); 
  }
  else if (WPM >= 25 && WPM <29){ //Print Level 6
    lcd.createChar(0, level6);  
    lcd.setCursor(13, 0);       
    lcd.write(byte(0)); 
  }
  else if (WPM >= 29 && WPM <33){ //Print Level 7
    lcd.createChar(0, level7);  
    lcd.setCursor(13, 0);       
    lcd.write(byte(0)); 
  }
  else if (WPM >= 33){            //Print Level 8
    lcd.createChar(0, level8);  
    lcd.setCursor(13, 0);       
    lcd.write(byte(0)); 
  }

  if (WPM >=5 && WPM < 10){
    lcd.setCursor(15,0);
    lcd.print(WPM);
  }
  else{
  lcd.setCursor(14, 0);
  lcd.print(WPM);
  }
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
