#include "./input.hpp"
#include "./EventHandler.hpp"
#include "memory.h"
#include <vector>
#include <cstring>

namespace ns {

namespace {
struct InputManagerEventHandler;

struct InputManagerState {
  bool keyboard[256];
  bool prev_keyboard[256];
  bool mouse[static_cast<usize>(Btn::MAX)];
  bool prev_mouse[static_cast<usize>(Btn::MAX)];
  i32 mouse_x;
  i32 mouse_y;
  i32 prev_mouse_x;
  i32 prev_mouse_y;
  Window* mouse_last_window_on;
  std::vector<InputManagerEventHandler> handlers;
};

InputManagerState *g_state = nullptr;

struct InputManagerEventHandler : public IEventHandler {
  Window* w;
  InputManagerEventHandler(Window *w) : IEventHandler(), w(w) {}
  virtual ~InputManagerEventHandler() {}
  bool handleKey(InputEvent t, Key k) override {
    if (t == InputEvent::PRESS) {
      g_state->keyboard[static_cast<usize>(k)] = true;
    }
    if (t == InputEvent::RELEASE) {
      g_state->keyboard[static_cast<usize>(k)] = false;
    }
    return false; // don't eat the event
  }
  bool handleMouseButton(InputEvent t, Btn b) override {
    if (t == InputEvent::PRESS) {
      g_state->mouse[static_cast<usize>(b)] = true;
    }
    if (t == InputEvent::RELEASE) {
      g_state->mouse[static_cast<usize>(b)] = false;
    }
    return false; // don't eat the event
  }
  bool handleMouseMotion(u32 x, u32 y) override {
    g_state->mouse_last_window_on = w;
    g_state->mouse_x = x;
    g_state->mouse_y = y;
    return false;
  }
};
} // namespace


void InputManager::initialize(usize *memory_requirement, ptr state) {
  *memory_requirement = sizeof(InputManagerState);
  if (state == nullptr) {
    return;
  }
  mem_zero(state, sizeof(InputManagerState));
  g_state = reinterpret_cast<InputManagerState*>(state);
}

void InputManager::cleanup([[maybe_unused]] ptr state) {
  g_state = nullptr;
}

void InputManager::update([[maybe_unused]] f64 delta_time) {
  if (!g_state) return;
  mem_copy(g_state->prev_keyboard, g_state->keyboard, sizeof(g_state->keyboard));
  mem_copy(g_state->prev_mouse, g_state->mouse, sizeof(g_state->mouse));
  g_state->prev_mouse_x = g_state->mouse_x;
  g_state->prev_mouse_y = g_state->mouse_y;
}

void InputManager::capture_window_events(Window* w) {
  g_state->handlers.emplace_back(w);
  w->addEventHandler(&g_state->handlers.back(), 50);
}

namespace keyboard {

bool pressed(Key k) {
  return g_state->keyboard[static_cast<usize>(k)] && !g_state->prev_keyboard[static_cast<usize>(k)];
}

bool released(Key k) {
  return !g_state->keyboard[static_cast<usize>(k)] && g_state->prev_keyboard[static_cast<usize>(k)];
}

bool down(Key k) {
  return g_state->keyboard[static_cast<usize>(k)];
}

bool up(Key k) {
  return !g_state->keyboard[static_cast<usize>(k)];
}

bool was_down(Key k) {
  return g_state->prev_keyboard[static_cast<usize>(k)];
}

bool was_up(Key k) {
  return !g_state->prev_keyboard[static_cast<usize>(k)];
}

} // namespace keyboard

namespace mouse {

bool pressed(Btn b) {
  return g_state->mouse[static_cast<usize>(b)] && !g_state->prev_mouse[static_cast<usize>(b)];
}

bool released(Btn b) {
  return !g_state->mouse[static_cast<usize>(b)] && g_state->prev_mouse[static_cast<usize>(b)];
}

bool down(Btn b) {
  return g_state->mouse[static_cast<usize>(b)];
}

bool up(Btn b) {
  return !g_state->mouse[static_cast<usize>(b)];
}

bool was_down(Btn b) {
  return g_state->prev_mouse[static_cast<usize>(b)];
}

bool was_up(Btn b) {
  return !g_state->prev_mouse[static_cast<usize>(b)];
}

void position(i32 &x, i32 &y) {
  x = g_state->mouse_x;
  y = g_state->mouse_y;
}

void prev_position(i32 &x, i32 &y) {
  x = g_state->prev_mouse_x;
  y = g_state->prev_mouse_y;
}

void motion(i32 &x, i32 &y) {
  x = g_state->mouse_x - g_state->prev_mouse_x;
  y = g_state->mouse_y - g_state->prev_mouse_y;
}

Window* last_window_on() {
  return g_state->mouse_last_window_on;
}

} // namespace mouse

} // namespace ns
