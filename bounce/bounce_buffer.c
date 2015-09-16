/*
    Code to read number of "bounces" on switch open or close.
    The data is buffered in-memory and dumped to the serial port
    after the switch has settled.

    Derived from "bounce.c".
*/

const int MaxBuffer = 1024;

void setup()
{
    Serial.begin(38400);
    pinMode(7, INPUT);
}

bool last_state = digitalRead(7);
unsigned long count = 0;
unsigned long buffer[MaxBuffer];
bool bool_buffer[MaxBuffer];
unsigned int ndx = 0;

void report(void)
{
    for (unsigned int i=0; i < ndx ; ++i)
    {
        Serial.print(buffer[i]);
        Serial.print(" samples with button ");
        if (bool_buffer[i])
            Serial.println("OPEN");
        else
            Serial.println("CLOSED");
    }

    ndx = 0;
}

void loop()
{
    bool state = digitalRead(7);

    if (state == last_state)
        count += 1;
    else
    {
        buffer[ndx] = count;
        bool_buffer[ndx] = last_state;
        ndx += 1;

        last_state = state;
        count = 1;
     }

     if (count > 2000000)
        report();
}
