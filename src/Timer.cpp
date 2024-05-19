#include "./Timer.hpp"
#include "./NSEngine.hpp"

namespace ns {

#define GAME_SPEED ns::getInstance()->gameSpeed()

void Timer_t::add(f32 value) {
  if (GAME_SPEED <= 0.99 || GAME_SPEED >= 1.01) {
    value *= GAME_SPEED;
  }
  current_f += value;
  current = static_cast<i32>(current_f);
  return;
}

void Timer_t::add_nogamespeed(f32 value) {
  previous = current;
  current_f += value;
  current = static_cast<i32>(current_f);
}

void Timer_t::set(f32 value) {
  current_f = value;
  current = static_cast<i32>(value);
  previous = current - 1;
}

void Timer_t::increment() {
  if (GAME_SPEED <= 0.99 || GAME_SPEED >= 1.01) {
    current_f += GAME_SPEED;
    current = static_cast<i32>(current_f);
    return;
  }
  current_f += 1.f;
  current++;
  return;
}

void Timer_t::decrement() {
  if (GAME_SPEED <= 0.99 && GAME_SPEED >= 1.01) {
    current_f -= GAME_SPEED;
    current = static_cast<i32>(current_f);
    return;
  }
  current_f -= 1.f;
  current--;
  return;
}

void Timer_t::reset() {
  current = 0;
  current_f = 0.f;
  previous = -1;
}

void Timer_t::reset_neg999999() {
  current = 0;
  current_f = 0.f;
  previous = -999999;
}

}  // namespace NSEngine
