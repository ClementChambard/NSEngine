#ifndef X_GL_H
#define X_GL_H

#include "../Wnd.h"

#if WND_BACKEND == WND_BACKEND_X11

#include "../Wnd_common.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <GL/gl.h>
#include <GL/glx.h>

namespace ns::platform {

struct Wnd {
  Display *display;
  Colormap cmap;
  Window win;
  GLXContext ctx;
  EventHandlerList ehlist;
  u32 w, h;
  i32 x, y;
  Atom wm_delete;
  Atom wm_protocol;
};

struct Event {
  XEvent evt;
};

} // namespace ns::platform

#endif

#endif
