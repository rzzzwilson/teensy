/*
    Code to count the number of "bounces" on switch open or close.
    The data is buffered in-memory and dumped to the serial port
    after the switch has settled.

    Derived from "bounce_buffer.c".
*/

// the pin the switch is attached to
const int SwitchPin = 7;
// store this number of results
const int MaxBuffer = 2048;
// after this number of samples, the switch has settled
const long int BounceLimit = 100000;

bool LastState = false;
unsigned long Count = 0;
int BounceCount = 0;
unsigned long CountBuffer[MaxBuffer];
bool BoolBuffer[MaxBuffer];
unsigned int Index = 0;

#define CHECK_MSEC        5        // Read hardware every 5 msec
#define PRESS_MSEC        10       // Stable time before registering pressed
#define RELEASE_MSEC      100      // Stable time before registering released

// This function reads the key state from the hardware.
bool RawKeyPressed(void)
{
    return digitalRead(SwitchPin);
}

// This holds the debounced state of the key.
bool DebouncedKeyPress = false;

// Service routine called every CHECK_MSEC to debounce both edges
void DebounceSwitch1(bool *Key_changed, bool *Key_pressed)
{
    static unsigned char Count = RELEASE_MSEC / CHECK_MSEC;
    bool RawState;

    *Key_changed = false;
    *Key_pressed = DebouncedKeyPress;

    RawState = RawKeyPressed();
    if (RawState == DebouncedKeyPress)
    {
        // Set the timer which will allow a change from the current state.
        if (DebouncedKeyPress)
            Count = RELEASE_MSEC / CHECK_MSEC;
        else
            Count = PRESS_MSEC / CHECK_MSEC;
    }
    else
    {
        // Key has changed - wait for new state to become stable.
        if (--Count == 0)
        {
            // Timer expired - accept the change.
            DebouncedKeyPress = RawState;
            *Key_changed=true;
            *Key_pressed = DebouncedKeyPress;

            // And reset the timer.
            if (DebouncedKeyPress)
                Count = RELEASE_MSEC / CHECK_MSEC;
            else
                Count = PRESS_MSEC / CHECK_MSEC;
        }
    }
}

// Service routine called by a timer interrupt
bool DebounceSwitch2()
{
    static unsigned int State = 0; // Current debounce status

    State = (State<<1) | !RawKeyPressed() | 0xe000;
    if (State==0xf000)
	return true;
    return false; 
}

void setup()
{
    Serial.begin(38400);
    pinMode(SwitchPin, INPUT);
    LastState = not DebounceSwitch2();
}

void report(void)
{
    for (unsigned int i=0; i < Index ; ++i)
    {
        unsigned long count = CountBuffer[i];
        bool state = BoolBuffer[i];

        Serial.print(count);
        if (count == 1)
            Serial.print(" change going to state ");
        else
            Serial.print(" changes going to state ");

        if (state)
            Serial.println("OPEN");
        else
            Serial.println("CLOSED");
    }

    Index = 0;
}

void loop()
{
    bool state = DebounceSwitch2();

    if (state == LastState)
    {
        if (Count == BounceLimit)
        {
            // switch has settled, save bounce count
            CountBuffer[Index] = BounceCount;
            BoolBuffer[Index] = state;
            ++Index;
            BounceCount = 0;
        }
    }
    else
    {
        ++BounceCount;
        LastState = state;
        Count = 0;
    }

    ++Count;

    if (Count > 2000000)
        report();
}
