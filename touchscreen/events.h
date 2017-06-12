#ifndef EVENTS_H
#define EVENTS_H

////////////////////////////////////////////////////////////////////////////////
// An event queue Class for PixelVFO.
////////////////////////////////////////////////////////////////////////////////

// define the VFOevents
enum Event
{
  event_None,
  event_Down,
  event_Up,
  event_Drag
};

// an event
typedef struct
{
  Event event;    // type of event
  int x;          // X coord for the event
  int y;          // Y coord for the event
} VFOEvent;


class Event
{
  public:
    Event(int size);
    ~Event(void);
    void push(Event event, int x, int y);
    VFOEvent *pop(void);
    int pending(void);
    void flush(void);
    void dump_queue(const char *msg);
    const char *display(VFOEvent *event);
  private:
    VFOEvent *event_queue; // pointer to the event circular buffer

    // queue pointers
    int queue_fore;   // index of the next event to be popped
    int queue_aft;    // index of the next free slot for a pushed event
}

#endif
