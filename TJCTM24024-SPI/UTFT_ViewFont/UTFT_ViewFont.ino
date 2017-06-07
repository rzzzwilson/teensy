#include <Adafruit_ILI9340.h>
#include <UTFT.h>
#include <memorysaver.h>

/*************************************************************************************
    UTFT_ViewFont (C)2013 Henning Karlsen
    web: http://www.henningkarlsen.com/electronics
    Fonts: http://www.henningkarlsen.com/electronics/r_fonts.php
  
    This program is a demo of the included fonts Plus a few downloaded ones.
    This demo was made for modules with a screen resolution 
    of 320x240 pixels.
    This program requires the UTFT library.
**************************************************************************************

    AliExpress ILI9341 2.2 Inch 240*320 SPI TFT LCD  Display
    http://www.aliexpress.com/item/Wholesale-1PC-2-2-Inch-240-320-Dots-SPI-TFT-LCD-Serial-Port-Module-Display-ILI9341/1188518505.html
    
    ILI9341 ............. Teensy 3.x
    VCC ................. 3.3V
    GND ................. Ground
    CS .................. 10
    RESET ............... 8
    D/C ................. 9
    SDO (MOSI) .......... 11
    SCK ................. 13
    LED ................. 100 Ohm -> 5V
    SDO (MISO) .......... 12

*************************************************************************************/


// Declare which fonts we will be using
extern uint8_t arial_bold[];
extern uint8_t Inconsola[];
extern uint8_t SevenSegNumFont[];

// ILI9341 on Teensy 3.1
UTFT myGLCD(ILI9341_S5P,11,13,10,8,9);   // Remember to change the model parameter to suit your display module!

void setup()
{
  Serial.begin(9600);
  myGLCD.InitLCD();
  myGLCD.clrScr();
}

void loop()
{
  for (int i=319; i>20; i=i-2)
  {
    myGLCD.setBackColor(0, 0, 0);
    myGLCD.setColor(255, 31, 31);
    myGLCD.setFont(SevenSegNumFont);
    myGLCD.setFont(arial_bold);
    myGLCD.print("ILI9341", i, 30, 0);       // Print String   x=i, y=30, rotation angle = 0
    myGLCD.print("Teensy 3.x", i, 50, 0);    // Print String   x=i, y=50, rotation angle = 0
  }
  Serial.println("Point 1");
  
  myGLCD.setColor(0, 255, 255);
  myGLCD.setFont(SevenSegNumFont);
  Serial.println("Point 2");

  for (int i=0; i<250; i++)
  {
    myGLCD.printNumI(i, 20, 90, 3, 48);    // Print Integer Number, x=20, y=90, 4 characters, 48= "0" (filler) 
  }
  Serial.println("Point 3");

  myGLCD.setColor(255, 127, 0);
  myGLCD.setFont(Inconsola);  
  Serial.println("Point 4");

#if 0
  for (float f=0; f<10000; f=f+0.01)
  {
    myGLCD.printNumF(f,2,20,170,46,8,48);    // Print Floating Number   2 decimal places, x=20, y=170, 46 = ".", 8 characters, 48= "0" (filler)
  }
  Serial.println("Point 5");
#endif
}
