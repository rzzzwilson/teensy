/*
    Code to test the button library code.
*/

#include "button.h"


// switch bounce settle time (milliseconds)
const unsigned long SettleTime = 10;

// the pin the switch is attached to
const int SwitchPin = 7;
//const int Switch2Pin = 8;

// the button ID
unsigned int ButtonID;


void setup()
{
    Serial.begin(115200);
    ButtonID = btn_register(SwitchPin, SettleTime);
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
    }

    delay(50);
}
