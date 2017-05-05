// The code here will:
//     print the 10 digits 0 to 9
//     select the 0 digit
//     move the selection with the rotary encoder

#include <LiquidCrystal.h>

// display constants - below is for ubiquitous small HD44780 display
#define NUM_ROWS        2
#define NUM_COLS        16

// max and min frequency showable
#define MAX_FREQ        30000000L
#define MIN_FREQ        1000000L

// size of frequency display in chars (30MHz is maximum frequency)
#define MAX_FREQ_CHARS  8

// number of the user-defined char we use for selected digit
#define SELECT_CHAR     0
#define SPACE_CHAR      1

// define the numeric digits with selection underline
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
byte selspace[8] = {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1f};

// array of references to the 10 'selected' numeric digit data
byte *sel_digits[] = {sel0, sel1, sel2, sel3, sel4, sel5, sel6, sel7, sel8, sel9, selspace};
#define SPACE_INDEX   10

// map cursor offset to bump values
unsigned long offset2bump[] = {1,         // offset = 0
                               10,        // 1
                               100,       // 2
                               1000,      // 3
                               10000,     // 4
                               100000,    // 5
                               1000000,   // 6
                               10000000}; // 7

// define the display connections
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

//------------------------------------------------------------------------------
// The standard Arduino setup() function.
//------------------------------------------------------------------------------
void setup()
{
  lcd.begin(NUM_COLS, NUM_COLS);      // define display size
  lcd.clear();
  lcd.createChar(SPACE_CHAR, sel_digits[SPACE_INDEX]);
}

//------------------------------------------------------------------------------
// Function to convert an unsigned long into an array of byte digit values.
//     buf      address of buffer for byte results
//     bufsize  size of the 'buf' buffer
//     value    the unsigned long value to convert
// The function won't overflow the given buffer, it will truncate at the left.
// 
// For example, given the value 1234 and a buffer of length 7, will fill the
// buffer with 0001234.  Given 123456789 it will fill with 3456789.
//------------------------------------------------------------------------------
void ltobbuff(char *buf, int bufsize, unsigned long value)
{
  char *ptr = buf + bufsize - 1;

  for (int i = 0; i < bufsize; ++i)
  {
    int rem = value % 10;
    
    value = value / 10;
    *ptr-- = char(rem);
  }
}

//------------------------------------------------------------------------------
// Print the frequency on the display with selected colum underlined.
//     freq  the frequency to display
//     col   the selection offset of digit to underline
//           (0 is rightmost digit, increasing to the left)
// The row and columns used to show frequency digits are defined elsewhere.
// A final "Hz" is written. 
//------------------------------------------------------------------------------
void print_freq(unsigned long freq, int col)
{
  char buf [MAX_FREQ_CHARS];
  int index = MAX_FREQ_CHARS - col - 1;
  bool lead_zero = true;

  ltobbuff(buf, MAX_FREQ_CHARS, freq);

  lcd.createChar(SELECT_CHAR, sel_digits[int(buf[index])]);
  lcd.setCursor(NUM_COLS - MAX_FREQ_CHARS - 2, 0);
  for (int i = 0; i < MAX_FREQ_CHARS; ++i)
  {
    int char_val = buf[i];

    if (char_val != 0)
        lead_zero = false;

    if (lead_zero)
    {
      if (index == i)
        lcd.write(byte(SPACE_CHAR));
      else
        lcd.write(" ");
    }
    else
    {
      if (index == i)
        lcd.write(byte(SELECT_CHAR));
      else
        lcd.write(char_val + '0');
    }
  }
  
  lcd.write("Hz");
}

//------------------------------------------------------------------------------
// Standard Arduino loop() function.
//------------------------------------------------------------------------------
void loop()
{
  static int offset = 3;
  static unsigned long freq = MIN_FREQ;
  
  delay(100);
  print_freq(freq, offset);
  ++freq;
}
