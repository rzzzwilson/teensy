Blink
=====

The teensy `tutorials <https://www.pjrc.com/teensy/tutorial.html>`_ start
with a 'blink' example which is a very simple *hello world!* example that
blinks the on-board LED.

**Blink.c** is the original teensy program, one second on, on second off.

**blink_double.c** blinks the LED twice rapidly with a one second repeat time.
Getting this to work exercises the Arduino build system.

**BlinkWithoutDelay.c** is a Blink.c variation where the current time is polled
in a loop allowing something to be done *besides* busy waiting.

**blink_without_delay_double.c** is my rewrite of *BlinkWithoutDelay.c* to
double blink the LED using the time poll approach.  This program uses a state
machine to handle the more complex double blink.  I've also added a second state
machine to handle printing something back to the serial monitor every 1.5
seconds.

Gripe
-----

The Arduino build system wants it's own filetypes - won't open and read a \*.c
file!  I have to cut and paste the code into the IDE!!  And I also have to cut
and paste changed code _back_ into the source code file as the IDE won't let me
save *anywhere* in the filesystem but only to a few selected places!!!  GRRRRR.

Must switch to a command line environment as soon as possible.
