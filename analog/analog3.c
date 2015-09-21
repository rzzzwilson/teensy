/*
    Analog input - read voltage and control LED flash rate.
*/

// the analog pin to read voltage from
const int analogInPin = A0;
const int ledPin = LED_BUILTIN;	// pin 13 for teensy 3.1

void setup()
{
    Serial.begin(9600);
    pinMode(ledPin, OUTPUT);
}

void loop()
{
    int pot = analogRead(analogInPin);
    int mapped = map(pot, 0, 1023, 20, 256);

    digitalWrite(ledPin, HIGH);
    delay(mapped);
    digitalWrite(ledPin, LOW);
    delay(mapped);

    // wait 2 milliseconds before the next loop - allow ADC to settle
    delay(2);
}
