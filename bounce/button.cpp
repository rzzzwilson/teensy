/*
    Code to test the button library code.
*/

#include <Arduino.h>
#include "button.h"

// struct used for each registered button
struct Button
{
    unsigned int pin;
    unsigned int settle;
    bool state;
    bool raw_state;
    unsigned long last_time;
    struct Button *next;
};
typedef Button BUTTON;

// list of BUTTON structs
static BUTTON *buttons = NULL;


/******************************************************************************
 * Interrupt service routine.  Process all registered buttons.
 ******************************************************************************/
static void btn_isr(void)
{
    cli();

    BUTTON *b = buttons;

    while (b)
    {
        // check status of pin
        bool state = digitalRead(b->pin);
       
        // if it has changed remember new state and time it changed 
        if (state != b->raw_state)
        {
            b->raw_state = state;
            b->last_time = millis();
        }

        b = b->next;
    }

    sei();
}

/******************************************************************************
 * Register pin as having pushbutton attached.
 *     pin     number of pin with button attached
 *     settle  settle time for the button
 * Returns the pushbutton ID (which is just the pin).
 ******************************************************************************/
unsigned int btn_register(unsigned int pin, unsigned int settle)
{
    BUTTON *new_button = (BUTTON *) malloc(sizeof(BUTTON));

    // fill in button information
    new_button->pin = pin;
    new_button->settle = settle;
    new_button->state = digitalRead(pin);
    new_button->raw_state = !new_button->state;
    new_button->last_time = millis();
    new_button->next = buttons;
    buttons = new_button;

    // make pin an input pin
    pinMode(pin, INPUT);

    // if state of pin changes call ISR routine
    attachInterrupt(pin, btn_isr, CHANGE);

    return pin;
}

/******************************************************************************
 * Get state of a button.
 *     id  ID number of the pin
 * Returns the pin settled (debounced) state.
 ******************************************************************************/
bool btn_state(unsigned int id)
{
    for (BUTTON *b = buttons; b != NULL; b = b->next)
    {
        if (b->pin == id)
        {
            if ((millis() - b->last_time) >= b->settle)
                b->state = b->raw_state;
            return b->state;    
        }
    }

    Serial.print("btn_state: Unrecognized BUTTON ID=");
    Serial.println(id);
    
    return false;
}
