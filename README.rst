teensy
======

A place for code I've written for the
`teensy <https://www.pjrc.com/teensy/teensy31.html>`_ microcontroller.

At first, this is just *paddling around, experimental* code using the
Arduino IDE and `teensyduino <https://www.pjrc.com/teensy/loader.html>`_
patch to handle the teensy 3.1 system.

Eventually I want to move to more UNIX-like 'make' system, but I can't
get that working with arduino-1.6.5-r5 yet.

Gripe
-----

The Arduino build system wants it's own filetypes - won't open and read a \*.c
file!  I have to cut and paste the code into the IDE!!  And I also have to cut
and paste changed code _back_ into the source code file as the IDE won't let me
save *anywhere* in the filesystem but only to a few selected places!!!  GRRRRR.

Must switch to a command line environment as soon as possible.
