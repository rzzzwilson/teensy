/*
    Code to test the button library code.
*/


// struct used for each registered button
typedef struct
{
    unsigned int pin;
    unsigned int settle;
    bool state;
    bool raw_state;
    unsigned long last_time;
} BUTTON;

// max number of buttons we can handle
static const int MAXBUTTONS = 10;

// array of BUTTON structs
static BUTTON buttons[MAXBUTTONS];

// next free slot in buttons[]
static unsigned int next_slot = 0;

/******************************************************************************
 * Interrupt service routine.  Process all registered buttons.
 ******************************************************************************/
static void btn_isr(void)
{
    cli();

    for (unsigned int i = 0; i < next_slot; ++i)
    {
        // check status of pin
        bool state = digitalRead(buttons[i].pin);
       
        // if it has changed remember new state and time it changed 
        if (state != buttons[i].raw_state)
        {
            buttons[i].raw_state = state;
            buttons[i].last_time = millis();
        }
    }

    sei();
}

/******************************************************************************
 * Register pin as having pushbutton attached.
 *     pin     number of pin with button attached
 *     settle  settle time for the button
 * Returns the pushbutton ID (index into buttons[]).
 ******************************************************************************/
unsigned int btn_register(unsigned int pin, unsigned int settle)
{
    if (next_slot == MAXBUTTONS)
    {
        // if no slots left, bleat and loop forever
        Serial.print("Too many buttons, max is ");
        Serial.println(MAXBUTTONS);
        while (true);
    }

    // fill in slot button information
    buttons[next_slot].pin = pin;
    buttons[next_slot].settle = settle;
    buttons[next_slot].state = digitalRead(pin);
    buttons[next_slot].raw_state = !buttons[next_slot].state;
    buttons[next_slot].last_time = millis();
    // make pin input pin
    pinMode(pin, INPUT);

    // if state of pin changes call ISR routine
    attachInterrupt(pin, btn_isr, CHANGE);

    return next_slot++;
}

/******************************************************************************
 * Get state of a button.
 *     id  ID number of the pin
 * Returns the pin settled (debounced) state.
 ******************************************************************************/
bool btn_state(unsigned int id)
{
    BUTTON button = buttons[id];

    if ((millis() - button.last_time) >= button.settle)
        button.state = button.raw_state;

    return button.state;
}


// switch bounce settle time (milliseconds)
const unsigned long SettleTime = 5;

// the pin the switch is attached to
const int SwitchPin = 7;
//const int Switch2Pin = 8;

// the button ID
unsigned int ButtonID;
//unsigned int Button2ID;


void setup()
{
    Serial.begin(115200);
    ButtonID = btn_register(SwitchPin, SettleTime);
//    Button2ID = btn_register(Switch2Pin, SettleTime);
};

void loop()
{
    bool state = btn_state(ButtonID);
    static bool old_state = !state;

    if (state != old_state)
    {
        Serial.print("New switch value=");
        Serial.println(state);
        old_state = state;
    }

    delay(50);
}
