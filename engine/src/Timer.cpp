#include "./Timer.hpp"
#include "./NSEngine.hpp"

#define GAME_SPEED ns::get_instance()->game_speed()

namespace ns {

void Timer_t::add(f32 value) {
  previous = current;
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
  previous = current;
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
  previous = current;
  if (GAME_SPEED <= 0.99 || GAME_SPEED >= 1.01) {
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

bool Timer_t::ticked() {
  return previous != current;
}

bool Timer_t::had_value(float val) {
  return (previous < val && current >= val) || (previous > val && current <= val);
}

u32 Timer_t::was_modulo(u32 value) {
  return count_true([value](i32 v) {
    return (static_cast<i32>(abs(v)) % value) == 0;
  });
}

bool Timer_t::had_true(std::function<bool(i32)> f) {
  i32 c = previous;
  while (c != current) {
    if (f(c)) return true;
    c++;
  }
  return false;
}

u32 Timer_t::count_true(std::function<bool(i32)> f) {
  u32 res = 0;
  i32 c = previous;
  while (c != current) {
    if (f(c)) res++;
    c++;
  }
  return res;
}

}  // namespace ns
