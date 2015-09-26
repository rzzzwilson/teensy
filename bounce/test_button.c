/*
    Code to test the button library code.
*/

#include "button.h"


// switch bounce settle time (milliseconds)
const unsigned long settleTime = 10;

// the pin the switch is attached to
const int switchPin = 7;

// the internal LED pin
const int ledPin = LED_BUILTIN;

// the button ID
unsigned int ButtonID;


void setup()
{
    Serial.begin(115200);
    ButtonID = btn_register(switchPin, settleTime);
    pinMode(ledPin, OUTPUT);
};

void loop()
{
    bool state = btn_state(ButtonID);
    static bool old_state = !state;

    if (state != old_state)
    {
        Serial.print("********************New switch value=");
        Serial.println(state);
        old_state = state;
        digitalWrite(ledPin, state);
    }

    delay(50);
}
