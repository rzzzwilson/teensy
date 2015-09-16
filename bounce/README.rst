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

One way around the time waste of printing is to buffer results into an
in-memory buffer and then dump the buffer after bouncing has stopped.
**bounce_buffer.c** attempts to do this.

