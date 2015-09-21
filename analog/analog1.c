/*
    Analog input - read voltage and print it.

    Code modified from: https://www.arduino.cc/en/Tutorial/AnalogInOutSerial
*/

// the analog pin to read voltage from
const int analogInPin = A0;
const int Delta = 1;

int potValue = 0;
int mappedValue = 0;

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    int pot_value = analogRead(analogInPin);
    int mapped_value = map(pot_value, 0, 1023, 0, 255);

    if (mapped_value != mappedValue)
    {
        Serial.print("pot value = ");
        Serial.print(pot_value);
        Serial.print(", mapped value = ");
        Serial.println(mapped_value);
        mappedValue = mapped_value;
    }

    // wait 2 milliseconds before the next loop
    // allows ADC to settle after the last reading
    delay(2);
}
