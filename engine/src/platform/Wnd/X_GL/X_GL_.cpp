#include "../Wnd.h"

#ifdef NS_WND_BACKEND_X11

#include "../Wnd.h"
#include "X_GL_.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../../../key.hpp"
#include "../../../memory.h"

namespace ns::platform {

#define GLX_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define GLX_CONTEXT_MINOR_VERSION_ARB 0x2092
typedef GLXContext (*glXCreateContextAttribsARBProc)(Display *, GLXFBConfig,
                                                     GLXContext, Bool,
                                                     const int *);
static bool ctxErrorOccured = false;
static int ctxErrorHandler([[maybe_unused]] Display *dpy, [[maybe_unused]] XErrorEvent *ev) {
  ctxErrorOccured = true;
  return 0;
}

static bool isExtensionSupported(char const *extList, char const *extension) {
  char const *start;
  char const *where, *terminator;

  where = strchr(extension, ' ');
  if (where || *extension == '\0') {
    return false;
  }

  for (start = extList;;) {
    where = strstr(start, extension);
    if (!where)
      break;
    terminator = where + strlen(extension);
    if ((where == start || *(where - 1) == ' ') &&
        (*terminator == ' ' || *terminator == '\0'))
      return true;

    start = terminator;
  }

  return false;
}

bool wnd_init(Wnd **wnd, char const *name, unsigned int width,
              unsigned int height, [[maybe_unused]] unsigned int flags) {
  // TODO: handle flags
  *wnd = ns::construct<Wnd, MemTag::APPLICATION>();
  Wnd &w = **wnd;

  w.w = width;
  w.h = height;
  w.x = 0;
  w.y = 0;

  w.display = XOpenDisplay(NULL);

  if (!w.display) {
    printf("Failed to open X display\n");
    ns::destroy<MemTag::APPLICATION>(*wnd);
    *wnd = nullptr;
    return false;
  }

  int glx_major, glx_minor;

  if (!glXQueryVersion(w.display, &glx_major, &glx_minor) ||
      ((glx_major == 1) && (glx_minor < 3)) || glx_major < 1) {
    printf("Invalid GLX version");
    XCloseDisplay(w.display);
    ns::destroy<MemTag::APPLICATION>(*wnd);
    *wnd = nullptr;
    return false;
  }

  printf("Getting matching framebuffer configs\n");
  int fbcount;
  static int visual_attribs[] = {
      GLX_X_RENDERABLE, True, GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
      GLX_RENDER_TYPE, GLX_RGBA_BIT, GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
      GLX_RED_SIZE, 8, GLX_GREEN_SIZE, 8, GLX_BLUE_SIZE, 8, GLX_ALPHA_SIZE, 8,
      GLX_DEPTH_SIZE, 24, GLX_STENCIL_SIZE, 8, GLX_DOUBLEBUFFER, True,
      // GLX_SAMPLE_BUFFERS, 1
      // GLX_SAMPLES, 4
      None};
  GLXFBConfig *fbc = glXChooseFBConfig(w.display, DefaultScreen(w.display),
                                       visual_attribs, &fbcount);
  if (!fbc) {
    printf("Failed to retrieve a framebuffer config\n");
    XCloseDisplay(w.display);
    ns::destroy<MemTag::APPLICATION>(*wnd);
    *wnd = nullptr;
    return false;
  }
  printf("Found %d matchig FB configs.\n", fbcount);
  printf("Getting XVisualInfos\n");
  int best_fbc = -1, worst_fbc = -1, best_num_samp = -1, worst_num_samp = 999;

  for (int i = 0; i < fbcount; i++) {
    XVisualInfo *vi = glXGetVisualFromFBConfig(w.display, fbc[i]);
    if (vi) {
      int samp_buf, samples;
      glXGetFBConfigAttrib(w.display, fbc[i], GLX_SAMPLE_BUFFERS, &samp_buf);
      glXGetFBConfigAttrib(w.display, fbc[i], GLX_SAMPLES, &samples);
      printf("  Matching fbconfig %d, visual ID 0x%2x: SAMPLE_BUFFERS = %d, "
             "SAMPLES = %d\n",
             i, (int)vi->visualid, samp_buf, samples);
      if (best_fbc < 0 || (samp_buf && samples > best_num_samp))
        best_fbc = i, best_num_samp = samples;
      if (worst_fbc < 0 || !samp_buf || samples < worst_num_samp)
        worst_fbc = i, worst_num_samp = samples;
    }
    XFree(vi);
  }

  GLXFBConfig bestFbc = fbc[best_fbc];

  XFree(fbc);

  XVisualInfo *vi = glXGetVisualFromFBConfig(w.display, bestFbc);
  printf("Chosen visual ID = 0x%2x\n", (int)vi->visualid);

  printf("Creating colormap\n");
  XSetWindowAttributes swa;
  swa.colormap = w.cmap = XCreateColormap(
      w.display, RootWindow(w.display, vi->screen), vi->visual, AllocNone);
  swa.background_pixmap = None;
  swa.border_pixel = 0;
  swa.event_mask = KeyPressMask | KeyReleaseMask | ButtonPressMask |
                   ButtonReleaseMask | StructureNotifyMask | PointerMotionMask |
                   ExposureMask;

  printf("Creating window\n");
  w.win = XCreateWindow(w.display, RootWindow(w.display, vi->screen), 0, 0,
                        width, height, 0, vi->depth, InputOutput, vi->visual,
                        CWBorderPixel | CWColormap | CWEventMask, &swa);
  if (!w.win) {
    printf("Failed to create window\n");
    XFreeColormap(w.display, w.cmap);
    XCloseDisplay(w.display);
    ns::destroy<MemTag::APPLICATION>(*wnd);
    *wnd = nullptr;
    return false;
  }

  XFree(vi);

  XStoreName(w.display, w.win, name);

  printf("Mapping window\n");
  XMapWindow(w.display, w.win);

  w.wm_protocol = XInternAtom(w.display, "WM_PROTOCOLS", False);
  w.wm_delete = XInternAtom(w.display, "WM_DELETE_WINDOW", False);
  XSetWMProtocols(w.display, w.win, &w.wm_delete, 1);
  // XSetWMProtocols(w.display, w.win, &w.wm_protocol, 1 );

  char const *glxExts =
      glXQueryExtensionsString(w.display, DefaultScreen(w.display));
  glXCreateContextAttribsARBProc glXCreateContextAttribsARB = 0;
  glXCreateContextAttribsARB =
      (glXCreateContextAttribsARBProc)glXGetProcAddressARB(
          (GLubyte const *)"glXCreateContextAttribsARB");

  w.ctx = 0;
  ctxErrorOccured = false;
  int (*oldHandler)(Display *, XErrorEvent *) =
      XSetErrorHandler(&ctxErrorHandler);

  if (!isExtensionSupported(glxExts, "GLX_ARB_create_context") ||
      !glXCreateContextAttribsARB) {
    printf("glXCreateContextAttribsARB() not found ... using old-style GLX "
           "context\n");
    w.ctx = glXCreateNewContext(w.display, bestFbc, GLX_RGBA_TYPE, 0, True);
  } else {
    int context_attribs[] = {GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
                             GLX_CONTEXT_MINOR_VERSION_ARB, 0, None};

    printf("Creating context\n");
    w.ctx = glXCreateContextAttribsARB(w.display, bestFbc, 0, True,
                                       context_attribs);
    XSync(w.display, False);
    if (!ctxErrorOccured && w.ctx) {
      printf("Created GL 3.0 context\n");
    } else {
      context_attribs[1] = 1;
      context_attribs[3] = 0;
      ctxErrorOccured = false;
      printf(
          "Failed to create GL 3.0 context ... using old-style GLX context\n");
      w.ctx = glXCreateContextAttribsARB(w.display, bestFbc, 0, True,
                                         context_attribs);
    }
  }

  XSync(w.display, False);

  XSetErrorHandler(oldHandler);

  if (ctxErrorOccured || !w.ctx) {
    printf("Failed to create OpenGL context\n");
    XDestroyWindow(w.display, w.win);
    XFreeColormap(w.display, w.cmap);
    XCloseDisplay(w.display);
    ns::destroy<MemTag::APPLICATION>(*wnd);
    *wnd = nullptr;
    return false;
  }

  if (!glXIsDirect(w.display, w.ctx)) {
    printf("Indirect GLX rendering context obtained\n");
  } else {
    printf("Direct GLX rendering context obtained\n");
  }

  printf("Making context current\n");
  glXMakeCurrent(w.display, w.win, w.ctx);

  return true;
}

void wnd_destroy(Wnd *wnd) {
  if (!wnd)
    return;
  glXMakeCurrent(wnd->display, 0, 0);
  glXDestroyContext(wnd->display, wnd->ctx);

  XDestroyWindow(wnd->display, wnd->win);
  XFreeColormap(wnd->display, wnd->cmap);
  XCloseDisplay(wnd->display);

  ns::destroy<MemTag::APPLICATION>(wnd);
}

void wnd_swap(Wnd *wnd) {
  if (!wnd)
    return;
  glXSwapBuffers(wnd->display, wnd->win);
}

void wnd_set_size(Wnd *wnd, unsigned int width, unsigned int height,
                  [[maybe_unused]] bool fullscreen) {
  if (!wnd)
    return;

  // FIX: on some WMs the window moves down the size of the title bar
  // * may be fixable getting parent window stats
  XMoveResizeWindow(wnd->display, wnd->win, 
                    wnd->x,
                    wnd->y,
                    width, height);
  if (wnd->w != width || wnd->h != height) {
    wnd->w = width;
    wnd->h = height;
    // TODO: fire resize event if it has changed.
  }
  // TODO: fullscreen
}

void wnd_set_mouse_pos(Wnd *wnd, unsigned int x, unsigned int y) {
  if (!wnd)
    return;
  XWarpPointer(wnd->display, None, wnd->win, 0, 0, 0, 0, x, y);
}

void wnd_add_event_handler(Wnd *wnd, IEventHandler *eventHandler,
                           unsigned int priority) {
  if (!wnd)
    return;
  wnd->ehlist.insert(eventHandler, priority);
}

Key translate_keycode(u32 x_keycode) {
  switch (x_keycode) {
  case XK_BackSpace:
    return Key::BACKSPACE;
  case XK_Return:
    return Key::RETURN;
  case XK_Tab:
    return Key::TAB;
  case XK_Pause:
    return Key::PAUSE;
  case XK_Caps_Lock:
    return Key::CAPITAL;
  case XK_Escape:
    return Key::ESCAPE;
  case XK_Mode_switch:
    return Key::MODECHANGE;
  case XK_space:
    return Key::SPACE;
  case XK_Prior:
    return Key::PRIOR;
  case XK_Next:
    return Key::NEXT;
  case XK_End:
    return Key::END;
  case XK_Home:
    return Key::HOME;
  case XK_Left:
    return Key::LEFT;
  case XK_Up:
    return Key::UP;
  case XK_Right:
    return Key::RIGHT;
  case XK_Down:
    return Key::DOWN;
  case XK_Select:
    return Key::SELECT;
  case XK_Print:
    return Key::PRINT;
  case XK_Execute:
    return Key::EXECUTE;
  case XK_Insert:
    return Key::INSERT;
  case XK_Delete:
    return Key::DELETE;
  case XK_Help:
    return Key::HELP;
  case XK_Meta_L:
    return Key::LWIN;
  case XK_Meta_R:
    return Key::RWIN;
  case XK_KP_Insert:
  case XK_KP_0:
    return Key::NUMPAD0;
  case XK_KP_End:
  case XK_KP_1:
    return Key::NUMPAD1;
  case XK_KP_Down:
  case XK_KP_2:
    return Key::NUMPAD2;
  case XK_KP_Next:
  case XK_KP_3:
    return Key::NUMPAD3;
  case XK_KP_Left:
  case XK_KP_4:
    return Key::NUMPAD4;
  case XK_KP_Begin:
  case XK_KP_5:
    return Key::NUMPAD5;
  case XK_KP_Right:
  case XK_KP_6:
    return Key::NUMPAD6;
  case XK_KP_Home:
  case XK_KP_7:
    return Key::NUMPAD7;
  case XK_KP_Up:
  case XK_KP_8:
    return Key::NUMPAD8;
  case XK_KP_Prior:
  case XK_KP_9:
    return Key::NUMPAD9;
  case XK_multiply:
    return Key::MULTIPLY;
  case XK_KP_Add:
    return Key::ADD;
  case XK_KP_Separator:
    return Key::SEPARATOR;
  case XK_KP_Subtract:
    return Key::SUBTRACT;
  case XK_KP_Delete:
  case XK_KP_Decimal:
    return Key::DECIMAL;
  case XK_KP_Divide:
    return Key::DIVIDE;
  case XK_KP_Multiply:
    return Key::MULTIPLY;
  case XK_KP_Equal:
    return Key::NUMPAD_EQUAL;
  case XK_F1:
    return Key::F1;
  case XK_F2:
    return Key::F2;
  case XK_F3:
    return Key::F3;
  case XK_F4:
    return Key::F4;
  case XK_F5:
    return Key::F5;
  case XK_F6:
    return Key::F6;
  case XK_F7:
    return Key::F7;
  case XK_F8:
    return Key::F8;
  case XK_F9:
    return Key::F9;
  case XK_F10:
    return Key::F10;
  case XK_F11:
    return Key::F11;
  case XK_F12:
    return Key::F12;
  case XK_F13:
    return Key::F13;
  case XK_F14:
    return Key::F14;
  case XK_F15:
    return Key::F15;
  case XK_F16:
    return Key::F16;
  case XK_F17:
    return Key::F17;
  case XK_F18:
    return Key::F18;
  case XK_F19:
    return Key::F19;
  case XK_F20:
    return Key::F20;
  case XK_F21:
    return Key::F21;
  case XK_F22:
    return Key::F22;
  case XK_F23:
    return Key::F23;
  case XK_F24:
    return Key::F24;
  case XK_Num_Lock:
    return Key::NUMLOCK;
  case XK_Scroll_Lock:
    return Key::SCROLL;
  case XK_Shift_L:
    return Key::LSHIFT;
  case XK_Shift_R:
    return Key::RSHIFT;
  case XK_Control_L:
    return Key::LCONTROL;
  case XK_Control_R:
    return Key::RCONTROL;
  case XK_Alt_L:
    return Key::LALT;
  case XK_Alt_R:
    return Key::RALT;
  case XK_semicolon:
    return Key::SEMICOLON;
  case XK_plus:
    return Key::PLUS;
  case XK_comma:
    return Key::COMMA;
  case XK_minus:
    return Key::MINUS;
  case XK_period:
    return Key::PERIOD;
  case XK_slash:
    return Key::SLASH;
  case XK_grave:
    return Key::GRAVE;
  case XK_equal:
    return Key::NUMPAD_EQUAL; // What ?
  case XK_bracketleft:
    return Key::LBRACKET;
  case XK_backslash:
    return Key::BACKSLASH;
  case XK_bracketright:
    return Key::RBRACKET;
  case XK_apostrophe:
    return Key::QUOTE;
  case XK_Menu:
    return Key::APPS;
  case XK_a:
  case XK_A:
    return Key::A;
  case XK_b:
  case XK_B:
    return Key::B;
  case XK_c:
  case XK_C:
    return Key::C;
  case XK_d:
  case XK_D:
    return Key::D;
  case XK_e:
  case XK_E:
    return Key::E;
  case XK_f:
  case XK_F:
    return Key::F;
  case XK_g:
  case XK_G:
    return Key::G;
  case XK_h:
  case XK_H:
    return Key::H;
  case XK_i:
  case XK_I:
    return Key::I;
  case XK_j:
  case XK_J:
    return Key::J;
  case XK_k:
  case XK_K:
    return Key::K;
  case XK_l:
  case XK_L:
    return Key::L;
  case XK_m:
  case XK_M:
    return Key::M;
  case XK_n:
  case XK_N:
    return Key::N;
  case XK_o:
  case XK_O:
    return Key::O;
  case XK_p:
  case XK_P:
    return Key::P;
  case XK_q:
  case XK_Q:
    return Key::Q;
  case XK_r:
  case XK_R:
    return Key::R;
  case XK_s:
  case XK_S:
    return Key::S;
  case XK_t:
  case XK_T:
    return Key::T;
  case XK_u:
  case XK_U:
    return Key::U;
  case XK_v:
  case XK_V:
    return Key::V;
  case XK_w:
  case XK_W:
    return Key::W;
  case XK_x:
  case XK_X:
    return Key::X;
  case XK_y:
  case XK_Y:
    return Key::Y;
  case XK_z:
  case XK_Z:
    return Key::Z;
  case XK_0:
    return Key::_0;
  case XK_1:
    return Key::_1;
  case XK_2:
    return Key::_2;
  case XK_3:
    return Key::_3;
  case XK_4:
    return Key::_4;
  case XK_5:
    return Key::_5;
  case XK_6:
    return Key::_6;
  case XK_7:
    return Key::_7;
  case XK_8:
    return Key::_8;
  case XK_9:
    return Key::_9;
  case XK_Super_L:
    return Key::LWIN;
  case XK_Super_R:
    return Key::RWIN;
  default:
    printf("Unhandled key_sym: 0x%04x\n", x_keycode);
    return Key::UNKNOWN;
  }
}

Btn translate_button(u32 x_btncode) {
  return static_cast<Btn>(x_btncode - 1);
}

void wnd_handle_events(Wnd *wnd) {
  if (!wnd)
    return;
  Event event;
  bool request_quit = false;
  while (XPending(wnd->display)) {
    XNextEvent(wnd->display, &event.evt);
    for (auto n = wnd->ehlist.head.next; n != &wnd->ehlist.tail; n = n->next) {
      if (n->eh->handle_platform_event(&event))
        break;
      auto evttyp = InputEvent::RELEASE;
      switch (event.evt.type) {
      case KeyPress:
        evttyp = InputEvent::PRESS;
        [[fallthrough]];
      case KeyRelease: {
        KeySym key_sym = XLookupKeysym(&event.evt.xkey, 0);
        if (n->eh->on_key(evttyp, translate_keycode(key_sym)))
          goto end;
      } break;
      case ButtonPress:
        evttyp = InputEvent::PRESS;
        [[fallthrough]];
      case ButtonRelease:
        // careful: mouse wheel is treated as button press.
        if (n->eh->on_mouse_button(
                evttyp, translate_button(event.evt.xbutton.button)))
          goto end;
        break;
      case MotionNotify:
        if (n->eh->on_mouse_motion(event.evt.xmotion.x, event.evt.xmotion.y))
          goto end;
        break;
      case ConfigureNotify:
        if (event.evt.xconfigure.width != static_cast<i32>(wnd->w) ||
            event.evt.xconfigure.height != static_cast<i32>(wnd->h)) {
          if (n->eh->on_resize(event.evt.xconfigure.width,
                                  event.evt.xconfigure.height)) {
            wnd->w = event.evt.xconfigure.width;
            wnd->h = event.evt.xconfigure.height;
            goto end;
          }
        }
        wnd->x = event.evt.xconfigure.x;
        wnd->y = event.evt.xconfigure.y;
        break;
      case ClientMessage:
        if (event.evt.xclient.data.l[0] == static_cast<long>(wnd->wm_delete)) {
          if (n->eh->on_quit())
            goto end;
          request_quit = true;
        }
        break;
        // TODO: other
      }
    }
  end:
    if (request_quit)
      return;
  }
}

} // namespace ns::platform

#endif
