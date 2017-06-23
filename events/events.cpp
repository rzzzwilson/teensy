////////////////////////////////////////////////////////////////////////////////
// An event queue class for PixelVFO.
//
// Implemented as a circular buffer.
// Since the code that pushes to the queue may be event-driven, we must be
// careful to disable/enable interrupts at the appropriate places.
////////////////////////////////////////////////////////////////////////////////

#include <Arduino.h>
#include "events.h"

// define this if the queue is cleared on creation
#define CLEAR_QUEUE


//----------------------------------------
// Create an instance of Events.
//----------------------------------------

Events::Events(int size)
{
  queue_size = size;
  
  // create the events buffer
  event_queue = (VFOEvent *) malloc(sizeof(VFOEvent) * size);

#ifdef CLEAR_QUEUE
  Serial.printf("Clearing the event queue\n");
  for (int i = 0; i < size; ++i)
  {
    event_queue[i].event = (Event) -1;
    event_queue[i].x = 0;
    event_queue[i].y = 0;
  }
#endif

  // set the queue to 'empty'
  clear();
}

//----------------------------------------
// Destroy an instance of Events.
//----------------------------------------

Events::~Events(void)
{
  // destroy the events buffer
  free(event_queue);
}

//----------------------------------------
// Convert an event to a display string.
// Used only for debug.
//----------------------------------------

const char *Events::display(VFOEvent *event)
{
  switch (event->event)
  {
    case event_None:
      return "{event_None, 0, 0}";
      break;
    case event_Down:
      sprintf(display_buff, "{event_Down, %d, %d}", event->x, event->y);
      break;
    case event_Up:
      sprintf(display_buff, "{event_Up, %d, %d}", event->x, event->y);
      break;
    case event_Drag:
      sprintf(display_buff, "{event_Drag, %d, %d}", event->x, event->y);
      break;
    default:
      sprintf(display_buff, "UNKNOWN EVENT");
  }
  
  return display_buff;
}

//----------------------------------------
// Push an event onto the event queue.
//     event  number of the event to push
//     x, y   the X and Y coords of event
// If queue is full, abort()!
//
// This routine is called only from interrupt code, so needs no protection.
//----------------------------------------

void Events::push(Event event, int x, int y)
{
  // put new event into next empty slot
  event_queue[queue_fore].event = event;
  event_queue[queue_fore].x = x;
  event_queue[queue_fore].y = y;

  // move fore ptr one slot up, wraparound if necessary
  ++queue_fore;
  if (queue_fore >= queue_size)
    queue_fore = 0;

  // if queue full, abort
  if (queue_aft == queue_fore)
  {
      dump_queue("ERROR: event queue full!");
      Serial.printf("Event queue full\n");

      // wait a bit here
      while (1);
  }
}

static VFOEvent NoneEvent = {event_None, 0, 0};

//----------------------------------------
// Pop next event from the queue.
//
// Returns event_None if queue is empty.
//----------------------------------------

VFOEvent *Events::pop(void)
{
  // Must protect from RE code fiddling with queue
  noInterrupts();

  // if queue empty, return None event
  if (queue_fore == queue_aft)
  {
    interrupts();
    return &NoneEvent;
  }

  // get next event
  VFOEvent *result = &event_queue[queue_aft];

  // move aft pointer up one slot, wrap if necessary
  ++queue_aft;
  if (queue_aft >= queue_size)
    queue_aft = 0;

  interrupts();

  return result;
}

//----------------------------------------
// Returns the number of events in the queue.
//----------------------------------------

int Events::len(void)
{
  // Must protect from RE code fiddling with queue
  noInterrupts();

  // get distance between fore and aft pointers
  int result = queue_fore - queue_aft;

  // handle case when events wrap around
  if (result < 0)
    result += queue_size;

  interrupts();

  return result;
}

//----------------------------------------
// Clear out any events in the queue.
//----------------------------------------

void Events::clear(void)
{
  // Must protect from code fiddling with queue
  noInterrupts();

  queue_fore = 0;
  queue_aft = 0;

  interrupts();
}

//----------------------------------------
// Dump the queue contents to the console.
//     msg  address of message to show
// Debug code.
//----------------------------------------

void Events::dump_queue(const char *msg)
{
  // Must protect from RE code fiddling with queue
  noInterrupts();

  Serial.printf(F("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"));
  Serial.printf(F("Queue: %s\n"), msg);
  for (int i = 0; i < queue_size; ++i)
  {
    VFOEvent *event = &event_queue[i];

    Serial.printf(F("  %d -> %s\n"), i, display(event));
  }
  if (len() == 0)
    Serial.printf(F("Queue length=0 (or %d)\n"), queue_size);
  else
    Serial.printf(F("Queue length=%d\n"), len());
  Serial.printf(F("queue_aft=%d"), queue_aft);
  Serial.printf(F(", queue_fore=%d\n"), queue_fore);
  Serial.printf(F("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"));

  interrupts();
}

