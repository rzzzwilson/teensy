#include <LiquidCrystal.h>

LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

byte smiley[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
};

byte fudge[8] = {
  B00000,
  B01110,
  B01110,
  B01110,
  B01110,
  B01110,
  B01110,
  B00000,
};

void setup()
{
  lcd.begin(16, 2); 
  lcd.createChar(1, fudge);
}

void loop()
{
  static int ch = 0;
  
  delay(100);
//  lcd.clear();
  lcd.createChar(ch, smiley);
  lcd.setCursor(0, 0);
  for (int i = 0; i < 16; ++i)
  {
     lcd.write(byte(ch));
  }
  lcd.createChar(ch+1, fudge);
  lcd.setCursor(0, 1);
  for (int i = 0; i < 16; ++i)
  {
     lcd.write(byte(ch+1));
  }

  if (++ch > 6)
    ch = 0;
}
