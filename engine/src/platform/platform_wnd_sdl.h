#ifndef PLATFORMWNDSDL_INCLUDED_H
#define PLATFORMWNDSDL_INCLUDED_H

#include "./platform_wnd.h"

// Only if using SDL

#include <SDL2/SDL.h>

namespace ns::platform {

struct Wnd {
  SDL_Window *window;
  SDL_GLContext glContext;
};

}

#endif // PLATFORMWNDSDL_INCLUDED_H
