#include <LiquidCrystal.h>

#define UNDER_CHAR  0


LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

byte sel0[8] = {0xe,0x11,0x13,0x15,0x19,0x11,0xe,0x1f};
byte sel1[8] = {0x4,0xc,0x4,0x4,0x4,0x4,0xe,0x1f};
byte sel2[8] = {0xe,0x11,0x1,0x2,0x4,0x8,0x1f,0x1f};
byte sel3[8] = {0x1f,0x2,0x4,0x2,0x1,0x11,0xe,0x1f};
byte sel4[8] = {0x2,0x6,0xa,0x12,0x1f,0x2,0x2,0x1f};
byte sel5[8] = {0x1f,0x10,0x1e,0x1,0x1,0x11,0xe,0x1f};
byte sel6[8] = {0x6,0x8,0x10,0x1e,0x11,0x11,0xe,0x1f};
byte sel7[8] = {0x1f,0x1,0x2,0x4,0x8,0x8,0x8,0x1f};
byte sel8[8] = {0xe,0x11,0x11,0xe,0x11,0x11,0xe,0x1f};
byte sel9[8] = {0xe,0x11,0x11,0xf,0x1,0x2,0xc,0x1f};

byte *sel_digits[] = {sel0, sel1, sel2, sel3, sel4, sel5, sel6, sel7, sel8, sel9};
void setup()
{
  lcd.begin(16, 2);     // define display size, 16 columns, 2 rows
}

// print data for char at 'offset'
void print_char(int offset)
{
  byte *bptr = sel_digits[offset];
  
  Serial.print("data: offset=");
  Serial.print(offset);
  Serial.print(" (");

  for (int i = 0; i < 8; ++i)
  {
    Serial.print(*bptr++, HEX);
    Serial.print(",");
  }
  Serial.println(")");
}

int offset = 0;

void loop()
{
  int col;

//  Serial.print("offset="); Serial.println(offset);
  print_char(offset);
  delay(500);
  lcd.clear();
  lcd.createChar(UNDER_CHAR, sel_digits[offset]);
  lcd.setCursor(6, 0);
  col = 6;
  for (int i = 0; i < 10; ++i)
  {
    Serial.print("    i="); Serial.print(i);
    Serial.print(", col="); Serial.print(col);
    if (offset == i)
    {
      lcd.write(byte(UNDER_CHAR));
      Serial.print(", char=*"); Serial.println(offset);
    }
    else
    {
      lcd.write(i + '0');
      Serial.print(", char="); Serial.println(i + '0');
    }
    ++col;
  }

  if (++offset > 9)
    offset = 0;
}
