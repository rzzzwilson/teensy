/**
 * XPT2046 Touch Screen Controller example.
 * 
 * Copyright (c) 02 Dec 2015 by Vassilis Serasidis
 * Home: http://www.serasidis.gr
 * email: avrsite@yahoo.gr
 * 
 * The sketch example has been created for using it with STM32Duino (http://www.stm32duino.com)
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON INFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <SPI.h>

#define csPin 4
#define Z_THRESHOLD   3500

int xy[2];

void setup()
{
  Serial.begin(115200);
  Serial.println("-------------------------------------------------");
  Serial.println("XPT2046 example sketch");
  Serial.println("Copyright (c) 02 Dec 2015 by Vassilis Serasidis");
  Serial.println("Home: http://www.serasidis.gr");
  Serial.println("-------------------------------------------------");
  
  SPI.begin(); //Initiallize the SPI1 port.
  pinMode(csPin, OUTPUT);
  digitalWrite(csPin, HIGH);
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void loop()
{
  if (read_XY(xy))  //Read the X,Y coordinates (12 bits, values 0-4095)
  {    
    Serial.print("X: ");
    Serial.print(xy[0]); //Print X value
    Serial.print(", Y: ");
    Serial.println(xy[1]); //Print Y value
    delay(1000);
  }
}

//-----------------------------------------------------
//
//-----------------------------------------------------
boolean read_XY(int *xy)
{
  int z1, z2, tmpH, tmpL;

  digitalWrite(csPin, LOW);

  //Check if touch screen is pressed.
  SPI.transfer(B10110001); // Z1
  tmpH = (SPI.transfer(0) << 5);
  tmpL = (SPI.transfer(0) >> 3);
  z1 = tmpH | tmpL;

  SPI.transfer(B11000001); // Z2
  tmpH = (SPI.transfer(0) << 5);
  tmpL = (SPI.transfer(0) >> 3);
  z2 = tmpH | tmpL;

  Serial.print("z1="); Serial.print(z1);
  Serial.print(", z2="); Serial.print(z2);
  Serial.print(", Z_THRESHOLD="); Serial.println(Z_THRESHOLD);
  
  if ((z2 - z1) < Z_THRESHOLD)  //If the touch screen is pressed, read the X,Y  coordinates from XPT2046.
  {
    SPI.transfer(B11010001); // X
    tmpH = (SPI.transfer(0) << 5);
    tmpL = (SPI.transfer(0) >> 3);
    xy[0] =  tmpH | tmpL;
    
    SPI.transfer(B10010001); // Y
    tmpH = (SPI.transfer(0) << 5);
    tmpL = (SPI.transfer(0) >> 3);
    xy[1] =  tmpH | tmpL;
    digitalWrite(csPin, HIGH);
    return true;
  }

  digitalWrite(csPin, HIGH);

  return false;
}
