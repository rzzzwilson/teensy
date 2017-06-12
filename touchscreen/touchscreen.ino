#include <SPI.h>
#include <XPT2046_Touchscreen.h>
#include "events.h"
#include "TouchScreen.h"


// SPI pins for touchscreen and interrupt pin
#define T_CS    4
#define T_IRQ   3


TouchScreen touch = TouchScreen(T_CS, T_IRQ, event_queue);

void vfo_abort(const char *msg)
{
  while (1);
}

void setup()
{
  Serial.begin(115200);
  Serial.println("Touchscreen test code"); 

  SPI.begin();
  TouchScreen(T_CS, T_IRQ, event_queue);
}

void loop(void)
{
  uint32_t now = millis();
  static uint32_t loop_msraw = 0;

  // ignore loop if too quick
  if ((now - loop_msraw) < 100) return;
  loop_msraw = now;

  // handle all events in the queue
  while (true)
  {
    // get next event and handle it
    VFOEvent *event = event_pop();

    if (event->event == event_None)
      break;

    Serial.printf("Event: %s\n", event2display(event));
  }
}
