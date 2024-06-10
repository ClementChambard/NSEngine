#ifndef SDL_GL_H
#define SDL_GL_H

#include "../Wnd.h"

#if WND_BACKEND == WND_BACKEND_SDL

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
