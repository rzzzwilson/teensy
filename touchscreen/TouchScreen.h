#ifndef TOUCHSCREEN_H
#define TOUCHSCREEN_H

#include <SPI.h>
#include <XPT2046_Touchscreen.h>
#include "events.h"

class TouchScreen
{
  public:
    TouchScreen(int t_cs, int t_irq, VFOEvent *queue);
  private:
    void read(void);

    int cs_pin;
    int irq_pin;

    uint32_t volatile msraw;
    uint16_t volatile last_xcalib;
    uint16_t volatile last_ycalib;
    bool volatile first_down;
    bool volatile PenDown;
    uint16_t volatile x_calib;
    uint16_t volatile y_calib;
}

#endif
