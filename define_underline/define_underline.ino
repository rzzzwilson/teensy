#include <LiquidCrystal.h>

LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

byte smiley[9] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
};

void setup() {
  lcd.createChar(0, smiley);
  lcd.begin(16, 2); 
  for (int i = 0; i < 16; ++i)
  {
     lcd.write(byte(0));
  }
  lcd.setCursor(0, 1);
  for (int i = 0; i < 16; ++i)
  {
     lcd.write(byte(0));
  }
}

void loop() {}
