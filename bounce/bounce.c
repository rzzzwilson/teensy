/*
    Code to read number of "bounces" on switch open or close.

    Derived from "Pushbutton", Teensyduino Tutorial #3
    http://www.pjrc.com/teensy/tutorial3.html
*/

void setup()
{
    Serial.begin(38400);
    pinMode(7, INPUT);
}

unsigned long count = 0;
bool last_state = digitalRead(7);

void loop()
{
    bool state = digitalRead(7);

    if (state == last_state)
    {
        count += 1;
    }
    else
    {
        Serial.print(count);
        Serial.print(" samples with button ");

        if (last_state)
            Serial.println("OPEN");
        else
            Serial.println("CLOSED");

        last_state = state;
        count = 1;
    }
}
