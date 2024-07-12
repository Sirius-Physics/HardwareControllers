#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);             // 20x4 LCD configuration
void lcdShow(int x, int y, String text, bool clearState){
  if (clearState){ lcd.clear(); }
  lcd.setCursor(x, y);
  lcd.print(text);
}

void lcdHomeScreen() {
  lcdShow(6, 0, "Waiting", true);
  lcdShow(9, 1, "for", false);
  lcdShow(5, 2, "Experience", false);
}

void lcdStartupConfig(){
  lcd.init();
  lcd.backlight();           
  lcdShow(3, 1, "SIRIUS STARTED", true);      // LCD signal SIRIUS system inicialization
  delay(4000);
  lcdHomeScreen();
}