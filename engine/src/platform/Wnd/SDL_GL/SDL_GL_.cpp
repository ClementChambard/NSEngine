#ifdef NS_WND_BACKEND_SDL2

#include "./SDL_GL_.h"
#include "../../../key.hpp"
#include "../../../memory.h"
#include "../Wnd.h"
#include "logger.h"
#include <SDL2/SDL_events.h>
#include <cstdio>

namespace ns::platform {

unsigned int translate_flags(unsigned int flags) {
  unsigned out = 0;
  if (flags & NS_WND_FULLSCREEN)
    out |= SDL_WINDOW_FULLSCREEN;

  return out | SDL_WINDOW_OPENGL;
}

static u32 wnd_cnt;

bool wnd_init(Wnd **wnd, char const *name, unsigned int width,
              unsigned int height, unsigned int flags) {
  if (wnd_cnt == 0 && SDL_Init(SDL_INIT_VIDEO) != 0) {
    NS_ERROR("Failed to initialize SDL!");
    return false;
  }

  if (wnd_cnt == 0) {
    NS_INFO("SDL initialized!");
  }

  wnd_cnt++;

  *wnd = ns::construct<Wnd>();
  Wnd &w = **wnd;

  w.window = SDL_CreateWindow(name, SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED, width, height,
                              translate_flags(flags) | SDL_WINDOW_RESIZABLE);

  if (!w.window) {
    NS_ERROR("Failed to create SDL window!");
    delete *wnd;
    *wnd = nullptr;
    return false;
  }

  w.glContext = SDL_GL_CreateContext(w.window);
  if (!w.glContext) {
    NS_ERROR("Failed to create GL context!");
    SDL_DestroyWindow(w.window);
    ns::destroy<MemTag::APPLICATION>(*wnd);
    *wnd = nullptr;
    return false;
  }

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetSwapInterval(1);

  return true;
}

void wnd_destroy(Wnd *wnd) {
  if (!wnd)
    return;
  SDL_GL_DeleteContext(wnd->glContext);
  SDL_DestroyWindow(wnd->window);
  if (--wnd_cnt == 0)
    SDL_Quit();
  ns::destroy(wnd);
}

void wnd_swap(Wnd *wnd) { SDL_GL_SwapWindow(wnd->window); }

void wnd_set_size(Wnd *pWnd, unsigned int width, unsigned int height,
                  bool fullscreen) {
  if (!pWnd)
    return;
  if (fullscreen)
    SDL_SetWindowFullscreen(pWnd->window, SDL_WINDOW_FULLSCREEN);
  else
    SDL_SetWindowFullscreen(pWnd->window, 0);
  SDL_SetWindowSize(pWnd->window, width, height);
}

void wnd_set_mouse_pos(Wnd *pWnd, unsigned int x, unsigned int y) {
  SDL_WarpMouseInWindow(pWnd->window, x, y);
}

void wnd_add_event_handler(Wnd *wnd, IEventHandler *eventHandler,
                           unsigned int priority) {
  if (!wnd)
    return;
  wnd->ehlist.insert(eventHandler, priority);
}

Key translate_keycode(u32 sdlk) {
  switch (sdlk) {
  case SDLK_BACKSPACE:
    return Key::BACKSPACE;
  case SDLK_TAB:
    return Key::TAB;
  case SDLK_CLEAR:
    return Key::CLEAR;
  case SDLK_RETURN:
    return Key::RETURN;
  case SDLK_PAUSE:
    return Key::PAUSE;
  case SDLK_CAPSLOCK:
    return Key::CAPITAL;
  case SDLK_ESCAPE:
    return Key::ESCAPE;
  case SDLK_MODE:
    return Key::MODECHANGE;
  case SDLK_SPACE:
    return Key::SPACE;
  case SDLK_PRIOR:
  case SDLK_PAGEUP:
    return Key::PRIOR;
  case SDLK_PAGEDOWN:
    return Key::NEXT;
  case SDLK_END:
    return Key::END;
  case SDLK_HOME:
    return Key::HOME;
  case SDLK_LEFT:
    return Key::LEFT;
  case SDLK_UP:
    return Key::UP;
  case SDLK_RIGHT:
    return Key::RIGHT;
  case SDLK_DOWN:
    return Key::DOWN;
  case SDLK_SELECT:
    return Key::SELECT;
  case SDLK_PRINTSCREEN:
    return Key::PRINT;
  case SDLK_EXECUTE:
    return Key::EXECUTE;
  case SDLK_INSERT:
    return Key::INSERT;
  case SDLK_DELETE:
    return Key::DELETE;
  case SDLK_HELP:
    return Key::HELP;
  case SDLK_0:
    return Key::_0;
  case SDLK_1:
    return Key::_1;
  case SDLK_2:
    return Key::_2;
  case SDLK_3:
    return Key::_3;
  case SDLK_4:
    return Key::_4;
  case SDLK_5:
    return Key::_5;
  case SDLK_6:
    return Key::_6;
  case SDLK_7:
    return Key::_7;
  case SDLK_8:
    return Key::_8;
  case SDLK_9:
    return Key::_9;
  case SDLK_a:
    return Key::A;
  case SDLK_b:
    return Key::B;
  case SDLK_c:
    return Key::C;
  case SDLK_d:
    return Key::D;
  case SDLK_e:
    return Key::E;
  case SDLK_f:
    return Key::F;
  case SDLK_g:
    return Key::G;
  case SDLK_h:
    return Key::H;
  case SDLK_i:
    return Key::I;
  case SDLK_j:
    return Key::J;
  case SDLK_k:
    return Key::K;
  case SDLK_l:
    return Key::L;
  case SDLK_m:
    return Key::M;
  case SDLK_n:
    return Key::N;
  case SDLK_o:
    return Key::O;
  case SDLK_p:
    return Key::P;
  case SDLK_q:
    return Key::Q;
  case SDLK_r:
    return Key::R;
  case SDLK_s:
    return Key::S;
  case SDLK_t:
    return Key::T;
  case SDLK_u:
    return Key::U;
  case SDLK_v:
    return Key::V;
  case SDLK_w:
    return Key::W;
  case SDLK_x:
    return Key::X;
  case SDLK_y:
    return Key::Y;
  case SDLK_z:
    return Key::Z;
  case SDLK_LGUI:
    return Key::LWIN;
  case SDLK_RGUI:
    return Key::RWIN;
  case SDLK_APPLICATION:
    return Key::APPS;
  case SDLK_SLEEP:
    return Key::SLEEP;
  case SDLK_KP_0:
    return Key::NUMPAD0;
  case SDLK_KP_1:
    return Key::NUMPAD1;
  case SDLK_KP_2:
    return Key::NUMPAD2;
  case SDLK_KP_3:
    return Key::NUMPAD3;
  case SDLK_KP_4:
    return Key::NUMPAD4;
  case SDLK_KP_5:
    return Key::NUMPAD5;
  case SDLK_KP_6:
    return Key::NUMPAD6;
  case SDLK_KP_7:
    return Key::NUMPAD7;
  case SDLK_KP_8:
    return Key::NUMPAD8;
  case SDLK_KP_9:
    return Key::NUMPAD9;
  case SDLK_ASTERISK:
  case SDLK_KP_MULTIPLY:
    return Key::MULTIPLY;
  case SDLK_PLUS:
    return Key::PLUS;
  case SDLK_SEPARATOR:
    return Key::SEPARATOR;
  case SDLK_MINUS:
    return Key::MINUS;
  case SDLK_PERIOD:
    return Key::PERIOD;
  case SDLK_SLASH:
    return Key::SLASH;
  case SDLK_F1:
    return Key::F1;
  case SDLK_F2:
    return Key::F2;
  case SDLK_F3:
    return Key::F3;
  case SDLK_F4:
    return Key::F4;
  case SDLK_F5:
    return Key::F5;
  case SDLK_F6:
    return Key::F6;
  case SDLK_F7:
    return Key::F7;
  case SDLK_F8:
    return Key::F8;
  case SDLK_F9:
    return Key::F9;
  case SDLK_F10:
    return Key::F10;
  case SDLK_F11:
    return Key::F11;
  case SDLK_F12:
    return Key::F12;
  case SDLK_F13:
    return Key::F13;
  case SDLK_F14:
    return Key::F14;
  case SDLK_F15:
    return Key::F15;
  case SDLK_F16:
    return Key::F16;
  case SDLK_F17:
    return Key::F17;
  case SDLK_F18:
    return Key::F18;
  case SDLK_F19:
    return Key::F19;
  case SDLK_F20:
    return Key::F20;
  case SDLK_F21:
    return Key::F21;
  case SDLK_F22:
    return Key::F22;
  case SDLK_F23:
    return Key::F23;
  case SDLK_F24:
    return Key::F24;
  case SDLK_NUMLOCKCLEAR:
    return Key::NUMLOCK;
  case SDLK_SCROLLLOCK:
    return Key::SCROLL;
  case SDLK_EQUALS:
    return Key::NUMPAD_EQUAL;
  case SDLK_LSHIFT:
    return Key::LSHIFT;
  case SDLK_RSHIFT:
    return Key::RSHIFT;
  case SDLK_LCTRL:
    return Key::LCONTROL;
  case SDLK_RCTRL:
    return Key::RCONTROL;
  case SDLK_LALT:
    return Key::LALT;
  case SDLK_RALT:
    return Key::RALT;
  case SDLK_SEMICOLON:
    return Key::SEMICOLON;
  case SDLK_KP_PLUS:
    return Key::ADD;
  case SDLK_COMMA:
    return Key::COMMA;
  case SDLK_KP_MINUS:
    return Key::SUBTRACT;
  case SDLK_KP_PERIOD:
    return Key::DECIMAL;
  case SDLK_KP_DIVIDE:
    return Key::DIVIDE;
  case SDLK_BACKQUOTE:
    return Key::GRAVE;
  case SDLK_LEFTBRACKET:
    return Key::LBRACKET;
  case SDLK_BACKSLASH:
    return Key::BACKSLASH;
  case SDLK_RIGHTBRACKET:
    return Key::RBRACKET;
  case SDLK_QUOTE:
    return Key::QUOTE;
  default:
    printf("Unhandled keycode: %d\n", sdlk);
    return Key::UNKNOWN;
  }
}

Btn translate_button(u32 sdlk) {
  switch (sdlk) {
  case SDL_BUTTON_LEFT:
    return Btn::LEFT;
  case SDL_BUTTON_RIGHT:
    return Btn::RIGHT;
  case SDL_BUTTON_MIDDLE:
    return Btn::MIDDLE;
  default:
    return static_cast<Btn>(sdlk);
  }
}

void wnd_handle_events(Wnd *pWnd) {
  if (!pWnd)
    return;
  Event e;
  while (SDL_PollEvent(&e.evt)) {
    for (auto n = pWnd->ehlist.head.next; n != &pWnd->ehlist.tail;
         n = n->next) {
      if (n->eh->handle_platform_event(&e))
        break;
      switch (e.evt.type) {
      case SDL_KEYUP:
        if (n->eh->on_key(InputEvent::RELEASE,
                          translate_keycode(e.evt.key.keysym.sym)))
          goto end;
        break;
      case SDL_KEYDOWN:
        if (n->eh->on_key(InputEvent::PRESS,
                          translate_keycode(e.evt.key.keysym.sym)))
          goto end;
        break;
      case SDL_MOUSEBUTTONUP:
        if (n->eh->on_mouse_button(InputEvent::RELEASE,
                                   translate_button(e.evt.button.button)))
          goto end;
        break;
      case SDL_MOUSEBUTTONDOWN:
        if (n->eh->on_mouse_button(InputEvent::PRESS,
                                   translate_button(e.evt.button.button)))
          goto end;
        break;
      case SDL_MOUSEMOTION:
        if (n->eh->on_mouse_motion(e.evt.motion.x, e.evt.motion.y))
          goto end;
        break;
      case SDL_QUIT:
        if (n->eh->on_quit())
          goto end;
        break;
      case SDL_WINDOWEVENT:
        if (e.evt.window.event == SDL_WINDOWEVENT_RESIZED) {
          if (n->eh->on_resize(e.evt.window.data1, e.evt.window.data2))
            goto end;
        }
        break;
        // TODO: more
      }
    }
  end:;
  }
}

} // namespace ns::platform

#endif
