/***************************************************
  This is our touchscreen painting example for the Adafruit ILI9341 Shield
  ----> http://www.adafruit.com/products/1651

  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/
/*
 * Hacked around by rzzzwilson
 */

#include <SPI.h>
#include <Adafruit_ILI9341.h>
#include <XPT2046_Touchscreen.h>


// This is calibration data for the raw touch data to the screen coordinates
#define TS_MINX 220
#define TS_MINY 340
#define TS_MAXX 3780
#define TS_MAXY 3850

// The XPT2046 uses hardware SPI on the shield, and #8, with #2 for interrupts
#define CS_PIN    4
//#define TIRQ_PIN  3
//XPT2046_Touchscreen ts(CS_PIN, TIRQ_PIN);
XPT2046_Touchscreen ts(CS_PIN);

// The display also uses hardware SPI, plus #9 & #10
#define TFT_DC 9
#define TFT_CS 10
#define TFT_RST 8
//#define TFT_LED 9   // LED goes to 3.3v

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);  

// Size of the color selection boxes and the paintbrush size
#define BOXSIZE 40
#define PENRADIUS 3
int oldcolor, currentcolor;

void setup(void)
{
  Serial.begin(115200);
  
//  pinMode(TFT_LED, OUTPUT);
//  digitalWrite(TFT_LED, HIGH);

  Serial.println("Touch Paint!");
  
  tft.begin();
  ts.begin();
  
  tft.setRotation(3);

  tft.fillScreen(ILI9341_BLACK);
  
  // make the color selection boxes
  tft.fillRect(0, 0, BOXSIZE, BOXSIZE, ILI9341_RED);
  tft.fillRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, ILI9341_YELLOW);
  tft.fillRect(BOXSIZE*2, 0, BOXSIZE, BOXSIZE, ILI9341_GREEN);
  tft.fillRect(BOXSIZE*3, 0, BOXSIZE, BOXSIZE, ILI9341_CYAN);
  tft.fillRect(BOXSIZE*4, 0, BOXSIZE, BOXSIZE, ILI9341_BLUE);
  tft.fillRect(BOXSIZE*5, 0, BOXSIZE, BOXSIZE, ILI9341_MAGENTA);
 
  // select the current color 'red'
  tft.drawRect(0, 0, BOXSIZE, BOXSIZE, ILI9341_WHITE);
  currentcolor = ILI9341_RED;
}

void loop()
{
  // See if there's any  touch data for us
  if (ts.bufferEmpty())
  {
    return;
  }

  // Retrieve a point  
  TS_Point p = ts.getPoint();

  if (p.z == 0)
  {
    return;
  }

  Serial.print(".width="); Serial.print(tft.width());
  Serial.print(", .height="); Serial.print(tft.height());
  Serial.print(", X="); Serial.print(p.x);
  Serial.print(", Y="); Serial.print(p.y);
  Serial.print(", Z="); Serial.println(p.z);  

  // Scale from ~0->4000 to tft.width using the calibration #'s
  p.x = 320 - map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
  p.y = 240 - map(p.y, TS_MINY, TS_MAXY, 0, tft.height());
  Serial.print("New p.x="); Serial.print(p.x);
  Serial.print(", p.y="); Serial.println(p.y);

  if (p.y < BOXSIZE)
  {
     oldcolor = currentcolor;

     if (p.x < BOXSIZE)
     { 
       currentcolor = ILI9341_RED; 
       tft.drawRect(0, 0, BOXSIZE, BOXSIZE, ILI9341_WHITE);
     }
     else if (p.x < BOXSIZE*2)
     {
       currentcolor = ILI9341_YELLOW;
       tft.drawRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, ILI9341_WHITE);
     }
     else if (p.x < BOXSIZE*3)
     {
       currentcolor = ILI9341_GREEN;
       tft.drawRect(BOXSIZE*2, 0, BOXSIZE, BOXSIZE, ILI9341_WHITE);
     }
     else if (p.x < BOXSIZE*4)
     {
       currentcolor = ILI9341_CYAN;
       tft.drawRect(BOXSIZE*3, 0, BOXSIZE, BOXSIZE, ILI9341_WHITE);
     }
     else if (p.x < BOXSIZE*5)
     {
       currentcolor = ILI9341_BLUE;
       tft.drawRect(BOXSIZE*4, 0, BOXSIZE, BOXSIZE, ILI9341_WHITE);
     }
     else if (p.x < BOXSIZE*6)
     {
       currentcolor = ILI9341_MAGENTA;
       tft.drawRect(BOXSIZE*5, 0, BOXSIZE, BOXSIZE, ILI9341_WHITE);
     }

     if (oldcolor != currentcolor)
     {
       if (oldcolor == ILI9341_RED) 
         tft.fillRect(0, 0, BOXSIZE, BOXSIZE, ILI9341_RED);
       if (oldcolor == ILI9341_YELLOW) 
         tft.fillRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, ILI9341_YELLOW);
       if (oldcolor == ILI9341_GREEN) 
         tft.fillRect(BOXSIZE*2, 0, BOXSIZE, BOXSIZE, ILI9341_GREEN);
       if (oldcolor == ILI9341_CYAN) 
         tft.fillRect(BOXSIZE*3, 0, BOXSIZE, BOXSIZE, ILI9341_CYAN);
       if (oldcolor == ILI9341_BLUE) 
         tft.fillRect(BOXSIZE*4, 0, BOXSIZE, BOXSIZE, ILI9341_BLUE);
       if (oldcolor == ILI9341_MAGENTA) 
         tft.fillRect(BOXSIZE*5, 0, BOXSIZE, BOXSIZE, ILI9341_MAGENTA);
     }
  }
  
  if (((p.y-PENRADIUS) > BOXSIZE) && ((p.y+PENRADIUS) < tft.height()))
  {
    tft.fillCircle(p.x, p.y, PENRADIUS, currentcolor);
  }
}
