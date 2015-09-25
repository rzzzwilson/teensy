/*
    Code to test the button library code.
*/


typedef struct
{
    unsigned int pin;
    unsigned int settle;
    bool state;
    bool raw_state;
    unsigned long last_time;
} BUTTON;

static const int MAXBUTTONS = 10;
static BUTTON buttons[MAXBUTTONS];
static unsigned int next_slot = 0;

static void btn_isr(void)
{
    cli();

    for (unsigned int i = 0; i < next_slot; ++i)
    {
        bool state = digitalRead(buttons[i].pin);
        
        if (state != buttons[i].raw_state)
        {
            buttons[i].raw_state = state;
            buttons[i].last_time = millis();
        }
    }

    sei();
}

//------------
// Register pin as having pushbutton attached.
// Returns the pushbutton ID.
//------------
unsigned int btn_register(unsigned int pin, unsigned int settle)
{
    if (next_slot == MAXBUTTONS)
    {
        Serial.print("Too many buttons, max is ");
        Serial.println(MAXBUTTONS);
        while (true);
    }

    buttons[next_slot].pin = pin;
    buttons[next_slot].settle = settle;
    buttons[next_slot].state = digitalRead(pin);
    buttons[next_slot].raw_state = !buttons[next_slot].state;
    buttons[next_slot].last_time = millis();
    pinMode(pin, INPUT);

    attachInterrupt(pin, btn_isr, CHANGE);

    return next_slot++;
}

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
const int Switch2Pin = 8;

// the button ID
unsigned int ButtonID;
unsigned int Button2ID;


void setup()
{
    Serial.begin(115200);
    ButtonID = btn_register(SwitchPin, SettleTime);
    Button2ID = btn_register(Switch2Pin, SettleTime);
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
