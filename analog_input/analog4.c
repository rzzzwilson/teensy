/*
    Analog input - read voltage and control LED flash rate.

    Use timer to control LED flash.
*/

// the analog pin to read voltage from
const int analogInPin = A0;
const int ledPin = LED_BUILTIN;	// pin 13 for teensy 3.1

// pot mapped value
int mappedValue = 0;
int lastMappedValue = -1;

//timer for LED flash
IntervalTimer myTimer;

// holds state of LED - true (ON) or false (OFF)
bool ledState = false;

void setup()
{
    Serial.begin(38400);
    pinMode(ledPin, OUTPUT);
    myTimer.begin(blinkLED, 9999999);
}

void blinkLED(void)
{
    if (ledState)
        digitalWrite(ledPin, LOW);
    else
        digitalWrite(ledPin, HIGH);
    ledState = !ledState;
}

void loop()
{
    mappedValue = map(analogRead(analogInPin), 0, 1023, 20, 500);
    if (abs(mappedValue - lastMappedValue) > 3)
    {
        myTimer.end();
	myTimer.begin(blinkLED, mappedValue*1000);
        lastMappedValue = mappedValue;
    }

    // wait before the next loop - allow ADC to settle
    delay(50);
}
