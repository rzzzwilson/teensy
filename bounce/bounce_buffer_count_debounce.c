/*
    Code to count the number of "bounces" on switch open or close.  The switch
    is software debounced.  The data is buffered in-memory and dumped to the
    serial port after the switch has settled.  We should see no bounces.

    The switch debounce code is test only.  We will create a class to wrap
    the debounce code.

    Derived from "bounce_buffer_count.c".
*/

#include <TimerOne.h>

//////////////////////////////////////////////////////////////
// values here are "switch" object variables
// will go into a class later

// Debounced switch state
bool DebouncedSwitchState = false;

// latest actual switch state
bool LastState = false;

// last state as found by timer routine
bool LastTimerState = true;

// debounce timer (microseconds)
const long DebounceTime = 10000;

// timer needs to see TimerTimes unchanged state
const int TimerTimes = 5;
int TimerCount = 0;

//////////////////////////////////////////////////////////////

// the pin the switch is attached to
const int SwitchPin = 7;
// store this number of results
const int MaxBuffer = 2048;

const long int BounceLimit = 100000;

unsigned long Count = 0;
int BounceCount = 0;
unsigned long CountBuffer[MaxBuffer];
bool BoolBuffer[MaxBuffer];
unsigned int Index = 0;

void myTimer(void)
{
    cli();
    bool state = digitalRead(7);
    if (state == LastTimerState)
    {
	if (++TimerCount > TimerTimes)
	{
	    // settled
            DebouncedSwitchState = state;
	    TimerCount = 0;
	}
    }
    else
    {
	TimerCount = 0;
        LastTimerState = state;
    }
    sei();
}

void isrCheckSwitch(void)
{
    cli();
    LastState = digitalRead(7);
    sei();
}

void setup()
{
    Serial.begin(115200);
    pinMode(SwitchPin, INPUT);
    attachInterrupt(SwitchPin, isrCheckSwitch, CHANGE);
    Timer1.initialize(DebounceTime);
    Timer1.attachInterrupt(myTimer);
}

void report(void)
{
    for (unsigned int i=0; i < Index ; ++i)
    {
        unsigned long count = CountBuffer[i];
        bool state = BoolBuffer[i];

        Serial.print(count);
        if (count == 1)
            Serial.print(" edge going to state ");
        else
            Serial.print(" edges going to state ");

        if (state)
            Serial.println("OPEN");
        else
            Serial.println("CLOSED");
    }

    Index = 0;
}

void loop()
{
    static bool last_state = false;

    if (DebouncedSwitchState == last_state)
    {
        if (Count == BounceLimit)
        {
            // switch has settled, save bounce count
            CountBuffer[Index] = BounceCount;
            BoolBuffer[Index] = DebouncedSwitchState;
            ++Index;
            BounceCount = 0;
        }
    }
    else
    {
        ++BounceCount;
        last_state = DebouncedSwitchState;
        Count = 0;
    }

    ++Count;

    if (Count > 2000000)
        report();
}
