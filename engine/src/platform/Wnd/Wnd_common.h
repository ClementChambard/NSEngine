#ifndef WND_COMMON_H
#define WND_COMMON_H

#include "../../EventHandler.hpp"

namespace ns::platform {

struct EventHandlerList {
  EventHandlerList();
  ~EventHandlerList();

  void insert(IEventHandler *eh, unsigned int priority);
  void remove(IEventHandler *eh);

  struct Node {
    IEventHandler *eh;
    unsigned int priority;
    Node *next;
    Node *prev;
  };
  Node head;
  Node tail;
};

} // namespace ns::platform

#endif
