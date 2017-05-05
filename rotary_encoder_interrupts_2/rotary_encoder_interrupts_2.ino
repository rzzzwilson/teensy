////////////////////////////////////////////////////////////////////////////////
// Interrupt driven rotary encoder interface.
////////////////////////////////////////////////////////////////////////////////

// define pins used by the rotary encoder
static int re_pinA = 2;     // encoder A pin
static int re_pinB = 3;     // encoder B pin
static int re_pinPush = 4;  // encoder pushbutton pin

// states for the encode machine
static int re_stateUp = 0;
static int re_stateDown = 1;
volatile int re_state = 0;

////////////////////////////
// internal state variables
////////////////////////////

// expecting rising edge on pinA - at detent
volatile byte aFlag = 0;

// expecting rising edge on pinA - at detent
volatile byte bFlag = 0;

// this variable stores our current value of encoder position.
// Change to int or uin16_t instead of byte if you want to record a larger range than 0-255
volatile byte re_position = 180;

// stores latest offset value
volatile int re_offset = 1;

// old values of 're_position' and 're_offset'
int oldEncOffset = 1;
byte oldEncPos = 180;

//----------------------------------------------------------
// Setup the encoder stuff, pins, etc.
//----------------------------------------------------------
void re_setup()
{
  // set pinA as an input, pulled HIGH to the logic voltage (5V or 3.3V for most cases)
  pinMode(re_pinA, INPUT_PULLUP);
  
  // set pinB as an input, pulled HIGH to the logic voltage (5V or 3.3V for most cases)
  pinMode(re_pinB, INPUT_PULLUP);
  
  // set pinPush as an input, pulled HIGH to the logic voltage (5V or 3.3V for most cases)
  pinMode(re_pinPush, INPUT_PULLUP);

  // attach pins to IST on rising edge only
  attachInterrupt(digitalPinToInterrupt(re_pinA), pinA_isr, RISING);
  attachInterrupt(digitalPinToInterrupt(re_pinB), pinB_isr, RISING);
  attachInterrupt(digitalPinToInterrupt(re_pinPush), pinPush_isr, CHANGE);
}

void bump_encoder_posn(int bump)
{
  if (re_state == re_stateUp)
  {
    re_position += bump;  
  }
  else if (re_state == re_stateDown)
  {
    re_offset -= bump;
    if (re_offset < 0)
      re_offset = 0;
    if (re_offset > 7)
      re_offset = 7;
    Serial.print("re_offset="); Serial.println(re_offset);
  }
}

void pinPush_isr()
{
  // stop interrupts, read Push pin, restore interrupts
  byte reading;

  cli();
    reading = PIND & 0x10;
    if (reading)
      re_state = re_stateUp;
    else
      re_state = re_stateDown;
  sei();
}

void pinA_isr()
{
  // stop interrupts, read A & B pins, restore interrupts
  byte reading;

  cli();
    reading = PIND & 0xC;

    if (reading == B00001100 && aFlag)
    { // check that we have both pins at detent (HIGH) and that we are expecting detent on
      // this pin's rising edge
      bump_encoder_posn(-1);    
      bFlag = 0; //reset flags for the next turn
      aFlag = 0; //reset flags for the next turn
    }
    else if (reading == B00000100)
      // show we're expecting pinB to signal the transition to detent from free rotation
      bFlag = 1;
  sei();
}

void pinB_isr()
{
  // stop interrupts, read A & B pins, restore interrupts
  byte reading;

  cli();
  reading = PIND & 0xC;
  
  if (reading == B00001100 && bFlag)
  { // check that we have both pins at detent (HIGH) and that we are expecting detent on
    // this pin's rising edge
    bump_encoder_posn(1);
    bFlag = 0; //reset flags for the next turn
    aFlag = 0; //reset flags for the next turn
  }
  else if (reading == B00001000)
    // show we're expecting pinA to signal the transition to detent from free rotation
    aFlag = 1;
  sei();
}

void setup()
{
  re_setup();
  Serial.begin(115200); // start the serial monitor link
}

void loop()
{
  byte copy_re_position;
  int copy_re_offset;
  int copy_re_state;

  // do an atomic copy of the volatile state variables
  cli();
    copy_re_position = re_position;
    copy_re_offset = re_offset;
    copy_re_state = re_state;
  sei();

  if (oldEncPos != copy_re_position || oldEncOffset != copy_re_offset)
  {
    if (copy_re_state == re_stateDown)
      Serial.print("* ");
    else
      Serial.print("  ");
    Serial.print("re_position="); Serial.print(copy_re_position);
    Serial.print(", re_offset="); Serial.println(copy_re_offset);
    
    oldEncPos = re_position;
    oldEncOffset = re_offset;
  }
}

