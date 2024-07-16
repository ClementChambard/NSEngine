#ifndef X_GL_H
#define X_GL_H

#ifdef NS_WND_BACKEND_X11

#include "../Wnd_common.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <GL/gl.h>
#include <GL/glx.h>

namespace ns::platform {

struct Wnd {
  Display *display = nullptr;
  Colormap cmap = 0;
  Window win = 0;
  GLXContext ctx = nullptr;
  EventHandlerList ehlist{};
  u32 w = 0, h = 0;
  i32 x = 0, y = 0;
  Atom wm_delete = 0;
  Atom wm_protocol = 0;
};

struct Event {
  XEvent evt;
};

} // namespace ns::platform

#endif

#endif
