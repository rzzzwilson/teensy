Analog
======

Here we have programs that demonstrate analog input.

**analog.c** is a program that reads the analog voltage on the A0 pin.  The
circuit used is:

.. image:: analog1_breadboard.png
    :width: 283
    :height: 224
    :alt: circuit used to test analog input (http://fritzing.org/)

**analog1.c** reads the voltage on A0 and serial.prints the value whenever
it changes.  Note that the value on pin A0 is a value in the range [0, 1023].
The code also maps the pin value to the range [0, 255] and prints only when
the mapped value changes.  Note that we get some *jitter* in the value as we
see the following when there is no physical movement in the potentiometer:

  | pot value = 747, mapped value = 186
  | pot value = 746, mapped value = 185
  | pot value = 747, mapped value = 186
  | pot value = 746, mapped value = 185
  | pot value = 747, mapped value = 186
  | pot value = 746, mapped value = 185
  | pot value = 747, mapped value = 186
  | pot value = 746, mapped value = 185
  | pot value = 747, mapped value = 186
  | pot value = 746, mapped value = 185

