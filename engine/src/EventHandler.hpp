#ifndef EVENTHANDLER_HEADER_INCLUDED
#define EVENTHANDLER_HEADER_INCLUDED

#include "./defines.h"

namespace ns {
  enum class Key;
  enum class Btn;
} // namespace ns

namespace ns::platform {
  struct Event;
} // namespace ns::platform

namespace ns {

enum class InputEvent {
  PRESS,
  RELEASE,
};

struct IEventHandler {
  virtual bool handle_platform_event(platform::Event* pe) { (void)pe; return false; } // Reserved to internal stuff since you have to know what is PlatformEvent
  virtual bool on_mouse_motion(u32 x, u32 y) { (void)x, (void)y; return false; }
  virtual bool on_mouse_button(InputEvent type, Btn button) { (void)type, (void)button; return false; }
  virtual bool on_key(InputEvent type, Key key) { (void)type, (void)key; return false; }
  virtual bool on_resize(u32 w, u32 h) { (void)w, (void)h; return false; }
  virtual bool on_quit() { return false; }
  // ... Other events
};

} // namespace ns

#endif // EVENTHANDLER_HEADER_INCLUDED
