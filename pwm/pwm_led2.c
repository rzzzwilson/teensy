/*
    PWM output - place a decreasing sawtooth voltage onto pin 10 (TX2).

    Use delay() calls in loop() to do this.
*/

// pin to read potentiometer voltage
const int analogInPin = A0;

// the PWM pin to write voltage to
const int pwmOutPin = 10;

// remember previous mapped value
int lastMappedValue = 0;
const int Delta = 2;


void setup()
{
    Serial.begin(38400);
    pinMode(analogInPin, INPUT);
    pinMode(pwmOutPin, OUTPUT);
}

void loop()
{
    int mapped_value = map(analogRead(analogInPin), 0, 1023, 0, 255);

    if (abs(mapped_value - lastMappedValue) >= Delta)
    {
        Serial.print("Writing value=");
        Serial.println(mapped_value);
	lastMappedValue = mapped_value;

        analogWrite(pwmOutPin, mapped_value);
    }

    delay(50);
}
