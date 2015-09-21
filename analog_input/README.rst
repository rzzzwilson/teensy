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
it changes.  Note that the value on pin A0 is in the range [0, 1023].
The code maps the pin value to the range [0, 255] and prints only when
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

**analog2.c** adds a little delta value and prints the values only if the 
absolute difference between the last mapped value and a new one is greater
than the delta.  A delta of 1 is enough to *smooth* the potentiometer values.

**analog3.c** takes the mapped potentiometer value and uses it to control the
flash rate of the builtin LED by code in loop().
