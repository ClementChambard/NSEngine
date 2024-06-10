#ifndef EVENTHANDLER_INCLUDED_H
#define EVENTHANDLER_INCLUDED_H

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
  virtual bool handlePlatformEvent(_ARG platform::Event* pe) { return false; } // Reserved to internal stuff since you have to know what is PlatformEvent
  virtual bool handleMouseMotion(_ARG u32 x, _ARG u32 y) { return false; }
  virtual bool handleMouseButton(_ARG InputEvent type, _ARG Btn button) { return false; }
  virtual bool handleKey(_ARG InputEvent type, _ARG Key key) { return false; }
  virtual bool handleResize(_ARG u32 w, _ARG u32 h) { return false; }
  virtual bool handleQuit() { return false; }
  // ... Other events
};

} // namespace ns

#endif // EVENTHANDLER_INCLUDED_H
