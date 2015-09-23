/*
    Analog output - place a symmetric sawtooth voltage onto pin A0.

    Use delay() calls in loop() to do this.
*/

// the analog pin to write voltage to
const int analogOutPin = A14;
// LED to flash every sawtooth cycle start
const int ledPin = LED_BUILTIN;        // pin 13 for teensy 3.1

// states of sawtooth wave
#define W_RISING	0
#define W_FALLING	1
bool waveState = W_RISING;

void setup()
{
    Serial.begin(38400);
    pinMode(ledPin, OUTPUT);
    pinMode(analogOutPin, OUTPUT);
    analogWriteResolution(8);
}

void blinkLED(void)
{
    digitalWrite(ledPin, HIGH);
    delay(100);
    digitalWrite(ledPin, LOW);
}

void loop()
{
    static int value = 0;

    Serial.print("Writing value=");
    Serial.println(value);
    analogWrite(analogOutPin, value);

    if (waveState == W_RISING)
    {
	if (++value >= 256)
	{
	    value = 255;
	    waveState = W_FALLING;
	}
    }
    else
    {
	if (--value <= 0)
	{
	    value = 0;
	    waveState = W_RISING;
	    blinkLED();
	}
    }
    delay(100);
}
