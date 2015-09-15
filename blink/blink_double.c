/*
    blink_double
    Turn on an LED twice rapidly and repeat on a 1 second cycle.
*/
 
// on a teensy 3.1 the LED is pin 13
int led = 13;

// the setup routine runs once when you press reset:
void setup()
{                
    // initialize the digital pin as an output.
    pinMode(led, OUTPUT);     
}

// the loop routine runs over and over again forever:
void loop()
{
    // rapid double flash of the LED
    digitalWrite(led, HIGH);
    delay(100);
    digitalWrite(led, LOW);
    delay(100);
    digitalWrite(led, HIGH);
    delay(100);
    digitalWrite(led, LOW);

    // we spent 300 msec above, wait 700 msec and repeat
    delay(700);
}
