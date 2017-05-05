#include <LiquidCrystal.h>

LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

byte smiley[8] = {
  B00000,
  B10001,
  B00000,
  B00000,
  B10001,
  B01110,
  B00000,
};

byte inv_smiley[8] = {
  B11111,
  B01110,
  B11111,
  B11111,
  B01110,
  B10001,
  B11111,
};

byte underline[8] = {0x1f,0x0,0x0,0x0,0x0,0x0,0x0};


void setup()
{
  lcd.begin(16, 2); 
  lcd.createChar(0, underline);
}

void show_frequency(long freq, int selected)
{
  
}

void loop()
{
  static int selected = 1;

//  lcd.clear();
  delay(500);
  
  lcd.setCursor(8, 0);
  for (int i = 1; i < 9; ++i)
  {
     lcd.write(i + '0');
  }
  lcd.setCursor(8, 1);
  for (int i = 1; i < 9; ++i)
  {
    if (i == selected)
      lcd.write(byte(0));
    else
      lcd.write(" ");
  }

  // move selection to next character
  if (++selected > 8)
      selected = 1;
}

