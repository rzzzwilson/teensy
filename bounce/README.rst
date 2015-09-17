Bounce
======

A mechanical pushbutton suffers from 'bounce'.  That is, the switch doesn't
behave as an ideal switch, but the circuit closes (or opens) with a series of
'bounces' before the final state of the switch is reached.

**bounce.c** is a program that checks the high/low status of pin 7 which is
attached to a pullup resistor and pushbutton pulling the resistor to ground.
The code quickly samples the state of pin 7 and counts the number of samples
before the state changes.  On a state change it prints the number of samples
in which the state was unchanged and the state itself.  This shows switch
bounce.  For example, the sample output below shows a switch initially closed.
Upon changing to the OPEN state we see 4 bounces before the switch stays OPEN.
The next state change (to CLOSED) sees 20 bounces!

  | 998305 samples with button CLOSED
  | 1 samples with button OPEN
  | 1 samples with button CLOSED
  | 40 samples with button OPEN
  | 1 samples with button CLOSED
  | 390703 samples with button OPEN
  | 1 samples with button CLOSED
  | 1 samples with button OPEN
  | 1 samples with button CLOSED
  | 4 samples with button OPEN
  | 8 samples with button CLOSED
  | 4 samples with button OPEN
  | 3 samples with button CLOSED
  | 1 samples with button OPEN
  | 21 samples with button CLOSED
  | 1 samples with button OPEN
  | 1 samples with button CLOSED
  | 22 samples with button OPEN
  | 2 samples with button CLOSED
  | 1 samples with button OPEN
  | 10 samples with button CLOSED
  | 1 samples with button OPEN
  | 32 samples with button CLOSED
  | 2 samples with button OPEN
  | 6 samples with button CLOSED
  | 1 samples with button OPEN
  | 4337362 samples with button CLOSED

The sample rate seems quite high, but this is C and not assembler.  Plus, upon
a state change we spend time writing strings to the serial port, probably missing
some bounce action.

One way around the time waste of printing is to store results into an
in-memory buffer and then dump the buffer after bouncing has stopped.
**bounce_buffer.c** attempts to do this.  Sample output shows:

  | 679250 samples with button CLOSED
  | 4 samples with button OPEN
  | 23 samples with button CLOSED
  | 520719 samples with button OPEN
  | 29 samples with button CLOSED
  | 1 samples with button OPEN
  | 5 samples with button CLOSED
  | 1 samples with button OPEN
  | 28 samples with button CLOSED
  | 1 samples with button OPEN
  | 26 samples with button CLOSED
  | 1 samples with button OPEN
  | 83 samples with button CLOSED
  | 1 samples with button OPEN
  | 11 samples with button CLOSED
  | 1 samples with button OPEN
  | 2 samples with button CLOSED
  | 1 samples with button OPEN
  | 16 samples with button CLOSED
  | 1 samples with button OPEN
  | 1 samples with button CLOSED
  | 5 samples with button OPEN
  | 49 samples with button CLOSED
  | 44 samples with button OPEN
  | 716 samples with button CLOSED
  | 21 samples with button OPEN
  | 623652 samples with button CLOSED
  | 1 samples with button OPEN
  | 25 samples with button CLOSED
  | 5 samples with button OPEN
  | 1 samples with button CLOSED
  | 1 samples with button OPEN
  | 29 samples with button CLOSED
  | 1 samples with button OPEN
  | 2 samples with button CLOSED
  | 2 samples with button OPEN
  | 1 samples with button CLOSED
  | 1 samples with button OPEN
  | 1 samples with button CLOSED
  | 4 samples with button OPEN
  | 2 samples with button CLOSED
  | 1 samples with button OPEN
  | 1 samples with button CLOSED

This shows some OPEN/CLOSE events as not bouncing at all.  Others bounce a lot.
One event was noticed with 50 bounces before settling!

**bounce_buffer_count.c** simplifies the above output by reporting on how many
bounces occurred before settling.  A state that remains unchanged for 100000
samples is considered settled.

  | 0 changes going to state OPEN
  | 3 changes going to state CLOSED
  | 1 change going to state OPEN
  | 3 changes going to state CLOSED
  | 1 change going to state OPEN
  | 7 changes going to state CLOSED
  | 1 change going to state OPEN
  | 7 changes going to state CLOSED
  | 1 change going to state OPEN
  | 3 changes going to state CLOSED
  | 1 change going to state OPEN
  | 5 changes going to state CLOSED
  | 1 change going to state OPEN
  | 5 changes going to state CLOSED
  | 1 change going to state OPEN

This shows the need for switch de-bounce!

**bounce_buffer_count_debounce.c** is derived from **bounce_buffer_count.c**
but includes software debounce before the test code.
