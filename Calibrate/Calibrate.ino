////////////////////////////////////////////////////////////////////////////////
// Program to calibrate a touchscreen on a 240x320 pixel TFT display.
//
// Run this, follow the instructions and copy the data into touchscreen code.
////////////////////////////////////////////////////////////////////////////////

#include <SPI.h>
#include <Adafruit_ILI9341.h>
#include <XPT2046_Touchscreen.h>

const char *ProgramName = "Calibrate";
const char *Version = "1.2";
const char *MinorVersion = "0";

// TFT display chip-select and data/control pins
#define TFT_RST     8
#define TFT_DC      9
#define TFT_CS      10

// pins for touchscreen and IRQ line
#define TS_CS       4
#define TS_IRQ      3

#define SCREEN_BG   ILI9341_BLACK
#define CROSS_FG    ILI9341_RED

// variables
typedef struct cal_data
{
  uint16_t x;
  uint16_t y;
  uint16_t raw_x;
  uint16_t raw_y;
} CalData;

#define NumCalPoints  9
CalData calibrate[NumCalPoints];

int cal_index = 0;    // index into calibrate[]

int state;
int count;

// Declare the display interface
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

// declare the touchscreen interface
XPT2046_Touchscreen ts(TS_CS);

//----------------------------------------
// Interrupt - pen went down (or up).
//
// Maintain the correct PenDown state.
//----------------------------------------

#if 0
void touch_isr(void)
{
  PenDown = (digitalRead(TS_IRQ) == 0);
  Serial.printf("%d:PenDown=%s\n", millis(), (PenDown) ? "true" : "false");
}
#endif

//-----------------------------------------------
// Initialize PixelVFO.
//-----------------------------------------------

void setup(void)
{
  // initialize the serial connection, announce we've started
  Serial.begin(115200);
  Serial.printf("%s %s.%s\n", ProgramName, Version, MinorVersion); 

  // initialize the pins, mode and level
  pinMode(TS_CS, OUTPUT);
  digitalWrite(TS_CS, HIGH);
  pinMode(TS_IRQ, INPUT);  // has external pullup & smothing

  // setup the target points array 'calibrate'
  int ndx = 0;
  int save_x = 0;
  int save_y = 0;
      
  for (int y = 0; y <= 240; y += 120)
  {
    for (int x = 0; x <= 320; x += 160)
    {
      save_x = x;
      save_y = y;
      if (save_x >= 320) save_x = 319;
      if (save_y >= 240) save_y = 239;
      calibrate[ndx].x = save_x;
      calibrate[ndx].y = save_y;
      calibrate[ndx].raw_x = 0;
      calibrate[ndx].raw_y = 0;
      ++ndx;
    }
  }
  cal_index = 0;

  count = 1;
  state = 0;

  // kick off the SPI system
  SPI.begin();

  // get the display ready
  tft.begin();
  tft.setRotation(1);

  // start drawing things that don't change
  tft.fillScreen(SCREEN_BG);
  tft.setTextWrap(false);

  // draw the initial target
  draw_target(cal_index);
}

void draw_target(int ndx)
{
  int x = calibrate[ndx].x;
  int y = calibrate[ndx].y;
  
  for (int i = 2; i <= 10; ++i)
    tft.fillCircle(x, y, i, ILI9341_RED);
}

void undraw_target(int ndx)
{
  int x = calibrate[ndx].x;
  int y = calibrate[ndx].y;
  
  for (int i = 2; i <= 10; ++i)
    tft.fillCircle(x, y, i, SCREEN_BG);
}

//-----------------------------------------------
// Main event loop is here.
//-----------------------------------------------

void loop(void)
{
  switch (state)
  {
    case 0:
      if (ts.touched())
      {
        TS_Point p = ts.getPoint();
        uint16_t pen_x = p.x;
        uint16_t pen_y = p.y;

        undraw_target(cal_index);
        calibrate[cal_index].raw_x = (int) (calibrate[cal_index].raw_x * (count-1)/count + pen_x/count);
        calibrate[cal_index].raw_y = (int) (calibrate[cal_index].raw_y * (count-1)/count + pen_y/count);
        state = 1;
      }
      break;
    case 1:
      if (! ts.touched())
      {
        if (++cal_index >= NumCalPoints)
        {
          Serial.printf("count=%d\n", count);
          for (int ndx = 0; ndx < NumCalPoints; ndx += 1)
          {
            Serial.printf("%02d: x=%3d, y=%3d, raw_x=%4d, raw_y=%4d\n",
                          ndx, calibrate[ndx].x, calibrate[ndx].y,
                          calibrate[ndx].raw_x, calibrate[ndx].raw_y); 
          }
          cal_index = 0;
          ++count;
        }
        state = 0;
        draw_target(cal_index);
      }
      break;
  }
}

