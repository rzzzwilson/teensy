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

// last actual switch state as found by interrupt routine
bool LastRawState = DebouncedSwitchState;

// latest _timer_ switch state
bool LastTimerState = !DebouncedSwitchState;
// debounce timer period (microseconds)
const long DebounceTime = 10000;
// timer needs to see TimerTimes unchanged state
const int TimerTimes = 5;
int TimerCount = 0;

void myTimer(void)
{
    cli();
    ++TimerCount;
    if (LastTimerState == LastRawState)
    {
        if (TimerCount == TimerTimes)
        {
            DebouncedSwitchState = LastTimerState;
//            save(TimerCount, LastTimerState, (char *) "Timer: new DebouncedSwitchState");
//            TimerCount = 0;
        }
    }
    else
    {
//        save(TimerCount, LastTimerState, (char *) "Timer: state change, new LastTimerState");
        TimerCount = 0;
        LastTimerState = LastRawState;
    }
    sei();
}

void isrCheckSwitch(void)
{
    cli();
    LastRawState = digitalRead(7);
//    save(0, LastRawState, (char *) "ISR: new LastRawState");
    sei();
}

//////////////////////////////////////////////////////////////
// normal code variables

// the pin the switch is attached to
const int SwitchPin = 7;
// store this number of results
const int MaxBuffer = 2048;

const long int BounceLimit = 100000;
const long int ReportTimeout = 1000000;

unsigned long Count = 0;
int BounceCount = 0;
unsigned long CountBuffer[MaxBuffer];
bool BoolBuffer[MaxBuffer];
char *OpBuffer[MaxBuffer];
unsigned int Index = 0;

//////////////////////////////////////////////////////////////
// Normal code below here

void setup()
{
    Serial.begin(115200);
    pinMode(SwitchPin, INPUT);

    // debounce service code below
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
        char *op = OpBuffer[i];

        Serial.print(count);
        if (count == 1)
            Serial.print(" edge going to state ");
        else
            Serial.print(" edges going to state ");

        if (state)
            Serial.print("OPEN, op=");
        else
            Serial.print("CLOSED, op=");

        Serial.println(op);
    }

    Index = 0;
}

void save(unsigned long count, bool state, char *op)
{
    CountBuffer[Index] = count;
    BoolBuffer[Index] = state;
    OpBuffer[Index] = op;
    ++Index;
}

void loop()
{
    static bool last_state = !DebouncedSwitchState;

    if (DebouncedSwitchState == last_state)
    {
        ++BounceCount;
        if (BounceCount == BounceLimit)
        {
            // switch has settled, save bounce count
            save(BounceCount, DebouncedSwitchState, (char *) "Change **********************");
            Count = 0;
        }
    }
    else
    {
        BounceCount = 0;
        Count = 0;
        last_state = DebouncedSwitchState;
    }

    ++Count;

    if (Count > ReportTimeout)
    {
        report();
	Count = 0;
    }
}
