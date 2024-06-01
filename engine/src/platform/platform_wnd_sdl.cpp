#include "./platform_wnd_sdl.h"
#include "./platform.h"
#include "../asserts.h"
#include "../logger.h"

namespace ns::platform {

u32 translate_flags(u32 flags) {
  u32 out = 0;
  if (flags & NS_WND_FULLSCREEN) out |= SDL_WINDOW_FULLSCREEN;

  return out | SDL_WINDOW_OPENGL;
}

void wnd_init(Wnd** ppWnd, cstr name, u32 width, u32 height, u32 flags) {
  NS_ASSERT_DEBUG(ppWnd != nullptr);
  Wnd* pWnd = reinterpret_cast<Wnd*>(allocate_memory(sizeof(Wnd), false));
  *ppWnd = pWnd;

  pWnd->window =
      SDL_CreateWindow(name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                       width, height, translate_flags(flags));
  if (pWnd->window == nullptr) {
    NS_FATAL("Failed to create window");
    exit(1);
  }

  pWnd->glContext = SDL_GL_CreateContext(pWnd->window);
  if (pWnd->glContext == nullptr) {
    NS_FATAL("Failed to create GL context");
    exit(1);
  }

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetSwapInterval(1);
}

void wnd_destroy(Wnd *pWnd) {
  SDL_GL_DeleteContext(pWnd->glContext);
  SDL_DestroyWindow(pWnd->window);
  platform::free_memory(pWnd, false);
}

void wnd_set_size(Wnd *pWnd, u32 width, u32 height, bool fullscreen) {
  SDL_SetWindowSize(pWnd->window, width, height);
  if (fullscreen)
    SDL_SetWindowFullscreen(pWnd->window, SDL_WINDOW_FULLSCREEN);
  else
    SDL_SetWindowFullscreen(pWnd->window, 0);
}

void wnd_swap_buffers(Wnd *pWnd) {
    SDL_GL_SwapWindow(pWnd->window);
}

void wnd_set_mouse_pos(Wnd *pWnd, u32 x, u32 y) {
    SDL_WarpMouseInWindow(pWnd->window, x, y);
}

}
