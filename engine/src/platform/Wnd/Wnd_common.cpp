#include "./Wnd_common.h"

// TODO: optimize node allocation

namespace ns::platform {

EventHandlerList::EventHandlerList() {
  head.eh = nullptr;
  head.priority = 0;
  head.next = &tail;
  head.prev = nullptr;
  tail.eh = nullptr;
  tail.priority = 999999;
  tail.next = nullptr;
  tail.prev = &head;
}

EventHandlerList::~EventHandlerList() {
  while (head.next != &tail) {
    Node* n = head.next;
    head.next = head.next->next;
    delete n;
  }
  tail.prev = &head;
}

void EventHandlerList::insert(IEventHandler *eh, unsigned int priority) {
  Node *n = head.next;
  while (n != &tail) {
    if (n->priority >= priority) break;
    n = n->next;
  }
  // insert before
  Node *new_node = new Node;
  n->prev->next = new_node;
  new_node->prev = n->prev;
  n->prev = new_node;
  new_node->next = n;
  new_node->eh = eh;
  new_node->priority = priority;
}

void EventHandlerList::remove(IEventHandler *eh) {
  Node *n = head.next;
  while (n != &tail) {
    if (n->eh == eh) break;
  }
  if (n == &tail) return;
  // remove node
  n->prev->next = n->next;
  n->next->prev = n->prev;
  delete n;
}

} // namespace ns::platform
