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


class Events
{
  public:
    Events(int size);
    ~Events(void);
    void push(Event event, int x, int y);
    VFOEvent *pop(void);
    int len(void);
    void clear(void);
    void dump_queue(const char *msg);
    const char *display(VFOEvent *event);

  private:
    int queue_size;         // size of the queue
    VFOEvent *event_queue;  // pointer to the event circular buffer
    int queue_fore;         // index of the next event to be popped
    int queue_aft;          // index of the next free slot for a pushed event
    char display_buff[64];  // buffer for display strings
};

#endif
