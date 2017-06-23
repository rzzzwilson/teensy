//////////////////////////////////////////////////////////////////////
// Test the Event code.
//////////////////////////////////////////////////////////////////////

#include "events.h"

void setup()
{
  Serial.begin(115200);
  Serial.println("Event test code");

  // create an event queue
  Events q = Events(10);
  q.dump_queue("Event queue created");

  q.push(event_Down, 10, 20);
  q.dump_queue("After pushing {event_Down, 10, 20}");

  q.push(event_Drag, 30, 40);
  q.dump_queue("After pushing {event_Drag, 30, 40}");

  VFOEvent *e = q.pop();
  q.dump_queue("Queue after pop()");
  Serial.printf("Popped event: %s\n", q.display(e));

  q.clear();
  q.dump_queue("Queue after a clear()");
}

void loop(void)
{
 
}
