/*
    Code to debounce a switch using the algorithm seen at:
    http://www.embedded.com/electronics-blogs/break-points/4024981/My-favorite-software-debouncers
    The routine used here is that called DebounceSwitch2() on the above page.

    The main loop() function is used as the timer.  Real production code would
    use the hardware timer(s).
*/


const int switchPin = 7;
const int ledPin = LED_BUILTIN;

bool switchState = true;

void checkSwitch(int pin, bool *state)
{
    static uint16_t internal_state = 0;

    internal_state = (internal_state<<1) | !digitalRead(pin) | 0xe000;
    if (internal_state==0xf000)
        *state = true;
    *state = false;
}

void setup()
{
    Serial.begin(38400);
    pinMode(switchPin, INPUT);
}

void loop()
{
    checkSwitch(switchPin, &switchState);

    if (switchState)
        digitalWrite(ledPin, HIGH);
    else
        digitalWrite(ledPin, LOW);

    delay(50);
}
