#ifndef SDL_GL_H
#define SDL_GL_H

#ifdef NS_WND_BACKEND_SDL2

#include "../Wnd_common.h"
#include <SDL2/SDL.h>

namespace ns::platform {

struct Wnd {
  SDL_Window *window;
  SDL_GLContext glContext;
  EventHandlerList ehlist;
};

struct Event {
  SDL_Event evt;
};

} // namespace ns::platform

#endif

#endif
