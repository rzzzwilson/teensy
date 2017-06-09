touchscreen
===========

The code here is exploratory code used to investigate how the XPT2046
touchscreen works.  In particular, how the T_IRQ line works.

I want to use touchscreen input for the PixelVFO and I want 'events'
from the touchscreen to be interrupt driven.  I fear that the Adafruit
XPT2046 driver will stop me from doing that.  Hence this test code.
