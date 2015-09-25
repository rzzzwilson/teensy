/*
    Code to test the button library code.
*/

#include <Arduino.h>


/******************************************************************************
 * Register pin as having pushbutton attached.
 *     pin     number of pin with button attached
 *     settle  settle time for the button
 * Returns the pushbutton ID (index into buttons[]).
 ******************************************************************************/
unsigned int btn_register(unsigned int pin, unsigned int settle);

/******************************************************************************
 * Get state of a button.
 *     id  ID number of the pin
 * Returns the pin settled (debounced) state.
 ******************************************************************************/
bool btn_state(unsigned int id);
