/*
    Double blink LED and repeat every second.

    Poll the time and do something if enough time has elapsed.  This allows the
    code to do something else (a bit) while waiting for the time to elapse
    without busy waiting.

    The "something else" here is another state machine printing USB debug every
    1.5 seconds.
*/

// constants defining the cycle time(s) (all in milliseconds)
const int LedOn = 100;
const int LedOff = 100;
const int CycleWait = 1000 - 2*(LedOn + LedOff);

const int UsbWait = 1500;

// teensy 3.1 has the LED on pin 13
const int ledPin = 13;

void setup()
{
    // set the digital pin as output:
    pinMode(ledPin, OUTPUT);      
    // prepare the serial port
    Serial.begin(38400);
}

void loop()
{
    // get current time
    long currentMillis = (long) millis();

    // pass current time to LED state machine
    led_state_machine(currentMillis);

    // and to the USB state machine
    usb_state_machine(currentMillis);
}

// a simple state machine to write debug to the USB port every 1.5 seconds
void usb_state_machine(long time)
{
    static long next_time = 0;
    static int state = 0;

    if (time > next_time)
    {
        switch (state)
        {
            case 0:                        // initialization
                next_time = time;
                /* FALL THROUGH */
            case 1:                        // write debug
                Serial.print(time, DEC);    
                Serial.println(" msec");                   
                next_time += UsbWait;
                state = 1;
                break;
        }
    }
}

// a simple state machine to flash the LED
void led_state_machine(long time)
{
    static long next_time = 0;
    static int state = 0;

    if (time > next_time)
    {
        switch (state)
        {
            case 0:                        // initialization
                next_time = time;
                /* FALL THROUGH */
            case 1:                        // first flash, ON
                digitalWrite(ledPin, HIGH);
                next_time += LedOn;
                state = 2;
                break;
            case 2:                        // first flash, OFF
                digitalWrite(ledPin, LOW);
                next_time += LedOff;
                state = 3;
                break;
            case 3:                        // second flash, ON
                digitalWrite(ledPin, HIGH);
                next_time += LedOn;
                state = 4;
                break;
            case 4:                        // second flash, OFF
                digitalWrite(ledPin, LOW);
                next_time += CycleWait;
                state = 1;
                break;
        }
    }
}
