/*
    Code to count the number of "bounces" on switch open or close.
    The data is buffered in-memory and dumped to the serial port
    after the switch has settled.

    Derived from "bounce_buffer.c".
*/

// store this number of results
const int MaxBuffer = 2048;
// after this number of samples, the switch has settled
const int BounceLimit = 1024;

void setup()
{
    Serial.begin(38400);
    pinMode(7, INPUT);
}

bool last_state = digitalRead(7);
unsigned long count = 0;
int bounce_count = 0;
unsigned long buffer[MaxBuffer];
bool bool_buffer[MaxBuffer];
unsigned int ndx = 0;

void report(void)
{
    for (unsigned int i=0; i < ndx ; ++i)
    {
        unsigned long count = buffer[i];
        bool state = bool_buffer[i];

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

    ndx = 0;
}

void loop()
{
    bool state = digitalRead(7);

    ++count;

    if (state == last_state)
    {
        if (count == BounceLimit)
        {
            // switch has settled, save bounce count
            buffer[ndx] = bounce_count;
            bool_buffer[ndx] = state;
            ++ndx;
            bounce_count = 0;
        }
    }
    else
    {
        ++bounce_count;
        last_state = state;
        count = 0;
    }

    if (count > 2000000)
        report();
}
