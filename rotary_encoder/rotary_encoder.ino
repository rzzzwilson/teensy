/*
** Rotary Encoder Example
** Use the Sparkfun Rotary Encoder to vary brightness of LED
**
** Sample the encoder at 200Hz using the millis() function
*/

// from: http://www.hobbytronics.co.uk/arduino-tutorial6-rotary-encoder
// using pins 21, 22 and 23

int brightness = 120;    // how bright the LED is, start at half brightness
int fadeAmount = 10;    // how many points to fade the LED by
unsigned long currentTime;
unsigned long loopTime;
const int pin_A = 22;
const int pin_B = 23;
const int pin_OUT = 21;
unsigned char encoder_A;
unsigned char encoder_B;
unsigned char encoder_A_prev=0;

// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

void setup()  {
  pinMode(pin_OUT, OUTPUT);
  pinMode(pin_A, INPUT);
  pinMode(pin_B, INPUT);
  currentTime = millis();
  loopTime = currentTime; 
  Serial.begin(115200);
  // set the brightness of pin ?:
  analogWrite(pin_OUT, brightness);
  Serial.println(brightness, DEC);
   
} 

void loop()
{
  // get the current elapsed time
  currentTime = millis();
  if (currentTime >= (loopTime + 2))
  {
    // 5ms since last check of encoder = 200Hz  
    encoder_A = digitalRead(pin_A);    // Read encoder pins
    encoder_B = digitalRead(pin_B);   
    if ((!encoder_A) && (encoder_A_prev))
    {
      // A has gone from high to low 
      if (encoder_B)
      {
        // B is high so clockwise
        // increase the brightness, dont go over 255
        if (brightness + fadeAmount <= 255)
          brightness += fadeAmount;               
      }   
      else
      {
        // B is low so counter-clockwise      
        // decrease the brightness, dont go below 0
        if (brightness - fadeAmount >= 0)
          brightness -= fadeAmount;               
      }   

      // set the brightness of pin ?:
      analogWrite(pin_OUT, brightness);
      Serial.println(brightness, DEC);
   
      lcd.clear();
      lcd.print(brightness);
    }   
    encoder_A_prev = encoder_A;     // Store value of A for next time    
    
    loopTime = currentTime;  // Updates loopTime
  }
  // Other processing can be done here
                           
}

