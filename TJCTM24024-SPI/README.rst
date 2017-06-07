TJCTM24024-SPI and Teensy 3.2
=============================

Here is the code I've written for the Teensy 3.2 running the 
`TJCTM24024-SPI LCD board with touch <https://www.google.co.th/search?q=TJCTM24024-SPI>`_
board.  I got mine from
`AliExpress <https://www.aliexpress.com/item/1pcs-J34-F85-240x320-2-8-SPI-TFT-LCD-Touch-Panel-Serial-Port-Module-with-PCB/32804198548.html?spm=2114.13010608.0.0.w3O1Gb>`_.

I had a bit of trouble getting the board to work probably due to my
insufficiently large brain coming to quick terms with the SPI interface.
Anyway, maybe this will help someone use this LCD display on a Teensy 3.2
(and probably a 3.1).

Caveats
-------

The display is a 3.3v item, so you must be careful using this with a 5v
Teensy or Arduino.

I have gotten the display itself to work, the touch screen is also working
(sort of, maybe needs calibration) and I haven't tried getting the SD card
to work, yet.

Wiring
------

All the code here assumes the following connections between the Teensy
and display:

+------------------------------+---------------------------------+
| Main display pins and labels | Teensy pins                     |
+==============================+=================================+
|  1. Vcc                      | 3.3v from Teensy                |
+------------------------------+---------------------------------+
|  2. GND                      | GND                             |
+------------------------------+---------------------------------+
|  3. CS                       | 10* (assumed by SPI library)    |
+------------------------------+---------------------------------+
|  4. RESET                    | 8 (or 3.3v if display ONLY)     |
+------------------------------+---------------------------------+
|  5. D/C                      | 9                               |
+------------------------------+---------------------------------+
|  6. SDI (MOSI)               | 11* (assumed by SPI library)    |
+------------------------------+---------------------------------+
|  7. SCK                      | 13* (assumed by SPI library)    |
+------------------------------+---------------------------------+
|  8. LED                      | 3.3v from Teensy                |
+------------------------------+---------------------------------+
|  9. SDO (MISO)               | 12* (assumed by SPI library)    |
+------------------------------+---------------------------------+
| 10. T_CLK                    | 13* (assumed by SPI library)    |
+------------------------------+---------------------------------+
| 11. T_CS                     | 4 (touch chip-select)           |
+------------------------------+---------------------------------+
| 12. T_DIN                    | 11* (assumed by SPI library)    |
+------------------------------+---------------------------------+
| 13. T_DO                     | 12* (assumed by SPI library)    |
+------------------------------+---------------------------------+
| 14. T_IRQ                    | 3 (used by XPT2046_Touchscreen) |
+------------------------------+---------------------------------+

The Teensy pins with asterisk (*) are assumed by the SPI library.
Other pins are at your discretion but if you change a pin number you
must make the appropriate change in the code.

The SD card connections tried are:

+------------------------------+---------------------------------+
| SD card pins                 | Teensy pins                     |
+==============================+=================================+
|  1. SD_CS                    | 2                               |
+------------------------------+---------------------------------+
|  2. SD_MOSI                  | 11* (assumed by SPI library)    |
+------------------------------+---------------------------------+
|  3. SD_MISO                  | 12* (assumed by SPI library)    |
+------------------------------+---------------------------------+
|  4. SD_SCK                   | 13* (assumed by SPI library)    |
+------------------------------+---------------------------------+

There seems to be some problem with the above setup running with 
*touchpaint*.  If there is no SD card, it works but sometines freezes
up drawing the initial screen.  With an SD card inserted, the screen stays white
and nothing else happens.  Try a simple SD-card only program.

Programs
--------

The *graphics_test* sketch was the first thing that worked.  It doesn't use the
touch interface.

The *touchpaint* sketch uses the touch screen.  Works, but there's quite a bit
of 'jitter' in the returned XY position.
