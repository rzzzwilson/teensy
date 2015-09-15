Blink
=====

The teensy `tutorials <https://www.pjrc.com/teensy/tutorial.html>`_ start
with a 'blink' example which is a very simple *hello world!* example that
blinks the on-board LED.

**blink.c** is the original teensy program, one second on, on second off.

**blink_double.c** blinks the LED twice rapidly with a one second repeat time.
Getting this to work exercises the Arduino build system.

The Arduino build system wants it's own filetypes - won't open and read a \*.c
file!  I have to cut and paste the code into the IDE.  And I also have to cut
and paste code _back_ into the source code file as the IDE won't let me save
anywhere in the filesyste but only to a few selected places.  GRRRRR.

Must switch to a command line environment as soon as possible.
