#include <XPT2046_Touchscreen.h>
#include <SPI.h>

#define CS_PIN  4
// MOSI=11, MISO=12, SCK=13

#define TIRQ_PIN  3
//XPT2046_Touchscreen ts(CS_PIN);         // Param 2 - NULL - No interrupts
//XPT2046_Touchscreen ts(CS_PIN, 255);    // Param 2 - 255 - No interrupts
XPT2046_Touchscreen ts(CS_PIN, TIRQ_PIN); // Param 2 - Touch IRQ Pin - interrupt enabled polling

void setup()
{
  Serial.begin(115200);
  ts.begin();
  while (!Serial && (millis() <= 1000));
  Serial.println("Touch the screen!");
}

void loopB()
{
  TS_Point p = ts.getPoint();
  Serial.print("Pressure = ");
  Serial.print(p.z);
  if (ts.touched())
  {
    Serial.print(", x = ");
    Serial.print(p.x);
    Serial.print(", y = ");
    Serial.print(p.y);
  }
  Serial.println();
  delay(30);
}

void loop()
{
  if (ts.touched())
  {
    TS_Point p = ts.getPoint();
    Serial.print("Pressure = ");
    Serial.print(p.z);
    Serial.print(", x = ");
    Serial.print(p.x);
    Serial.print(", y = ");
    Serial.print(p.y);
    Serial.println();
    delay(30);
  }
}

