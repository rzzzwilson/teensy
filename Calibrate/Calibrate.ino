////////////////////////////////////////////////////////////////////////////////
// Program to calibrate a touchscreen on a 240x320 pixel TFT display.
//
// Run this, follow the instructions and copy the data into touchscreen code.
////////////////////////////////////////////////////////////////////////////////

#include <SPI.h>
//#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <XPT2046_Touchscreen.h>

const char *ProgramName = "Calibrate";
const char *Version = "1.0";
const char *MinorVersion = "1";

// TFT display chip-select and data/control pins
#define TFT_RST 8
#define TFT_DC  9
#define TFT_CS  10

// pins for touchscreen and IRQ line
#define T_CS    4
#define T_IRQ   3

#define SCREEN_BG           ILI9341_BLACK
#define CROSS_FG            ILI9341_RED

#define SPI_SETTING         SPISettings(2000000, MSBFIRST, SPI_MODE0)

#define MIN_REPEAT_PERIOD   20

// Declare the display interface
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

// variables
bool PenDown = false;

int minx = 5000;
int miny = 5000;
int maxx = -1;
int maxy = -1;

int last_minx = 5000;
int last_miny = 5000;
int last_maxx = -1;
int last_maxy = -1;


//-----------------------------------------------
// Read touch screen, return raw coordinates if touched.
//
//     x, y  address of X & Y coordinates to update
//
// Returns 'true' if touched, X and Y updated
//-----------------------------------------------

bool touch_read(int *x, int *y)
{
  // if finger not down, do nothing
  if (! PenDown) return false;

  // otherwise, read the device
  SPI.beginTransaction(SPI_SETTING);
  digitalWrite(T_CS, LOW);      // enable comms with slave

  SPI.transfer16(0x91 /* X */);   // we seem to read a few times
  SPI.transfer16(0xD1 /* Y */);
  SPI.transfer16(0x91 /* X */);
  SPI.transfer16(0xD1 /* Y */);
  SPI.transfer16(0x91 /* X */);
  *x = SPI.transfer16(0xD0 /* Y */) >> 3;
  *y = SPI.transfer16(0) >> 3;

  digitalWrite(T_CS, HIGH);     // finished comms with slave
  SPI.endTransaction();

  return true;
}

//----------------------------------------
// Interrupt - pen went down or up.
//
// Maintain the correct PenDown state and call touch_read() if DOWN.
// Also push a event_Up event if going UP.
//----------------------------------------

void touch_isr(void)
{
  static uint32_t  msraw = 0L;
  uint32_t now = millis();

  // ignore interrupt if too quick
  if ((now - msraw) < MIN_REPEAT_PERIOD) return;
  msraw = now;

  PenDown = (digitalRead(T_IRQ) == 0);
}

//-----------------------------------------------
// Initialize PixelVFO.
//-----------------------------------------------

void setup(void)
{
  // initialize the serial connection, announce we've started
  Serial.begin(115200);
  Serial.printf("%s %s.%s\n", ProgramName, Version, MinorVersion); 

  // initialize the pins, mode and level
  pinMode(T_CS, OUTPUT);
  digitalWrite(T_CS, HIGH);
  pinMode(T_IRQ, INPUT_PULLUP);

  // initiaize some state
  PenDown = false;

  // kick off the SPI system
  SPI.begin();

  // get the display ready
  tft.begin();
  tft.setRotation(1);

  // start drawing things that don't change
  tft.fillScreen(SCREEN_BG);
  tft.setTextWrap(false);

  // link the irq_pin pint to it's interrupt handler
  attachInterrupt(digitalPinToInterrupt(T_IRQ), touch_isr, CHANGE);
}

//-----------------------------------------------
// Main event loop is here.
//-----------------------------------------------

void loop(void)
{
  int pen_x;
  int pen_y;

  if (touch_read(&pen_x, &pen_y))
  {
    if (pen_x < minx) minx = pen_x;
    if (pen_y < miny) miny = pen_y;
    if (pen_x > maxx) maxx = pen_x;
    if (pen_y > maxy) maxy = pen_y;
  }

  if (minx != last_minx || miny != last_miny || maxx != last_maxx || maxy != last_maxy)
  {
    Serial.printf("// touchscreen calibration data\n");
    Serial.printf("#define TS_MINX %d\n", minx);
    Serial.printf("#define TS_MINY %d\n", miny);
    Serial.printf("#define TS_MAXX %d\n", maxx);
    Serial.printf("#define TS_MAXY %d\n\n", maxy);

    last_minx = minx;
    last_miny = miny;
    last_maxx = maxx;
    last_maxy = maxy;
  }
}
