#ifndef TIMER_HEADER_INCLUDED
#define TIMER_HEADER_INCLUDED

#include "./defines.h"
#include <compare>
#include <functional>

namespace ns {

struct Timer_t {
  Timer_t() {}
  Timer_t(f32 c) { set(c); }
  Timer_t(i32 c) { set(c); }

  void add(f32 value);
  void add_nogamespeed(f32 value);
  void set(f32 value);
  void increment();
  void decrement();
  void reset();
  void reset_neg999999();
  bool ticked();
  bool had_value(f32 value);
  u32 was_modulo(u32 value);
  bool had_true(std::function<bool(i32)> f);
  u32 count_true(std::function<bool(i32)> f);

  Timer_t &operator=(i32 c) {
    set(c);
    return *this;
  }
  Timer_t &operator=(u32 c) {
    set(c);
    return *this;
  }
  Timer_t &operator=(f32 c) {
    set(c);
    return *this;
  }

  Timer_t &operator++() {
    increment();
    return *this;
  }
  Timer_t operator++(i32) {
    auto o = *this;
    increment();
    return o;
  }
  Timer_t &operator--() {
    decrement();
    return *this;
  }
  Timer_t operator--(i32) {
    auto o = *this;
    decrement();
    return o;
  }
  Timer_t& operator+=(i32 v) {
    add(v);
    return *this;
  }
  Timer_t& operator+=(u32 v) {
    add(v);
    return *this;
  }
  Timer_t& operator+=(f32 v) {
    add(v);
    return *this;
  }
  Timer_t& operator-=(i32 v) {
    add(-v);
    return *this;
  }
  Timer_t& operator-=(u32 v) {
    add(-static_cast<i32>(v));
    return *this;
  }
  Timer_t& operator-=(f32 v) {
    add(-v);
    return *this;
  }

  i32 previous = -1;
  i32 current = 0;
  f32 current_f = 0.f;
  // f32* __game_speed__disused = nullptr;
  // i32 control = 0;
};

inline auto operator<=>(Timer_t const &t1, Timer_t const &t2) { return t1.current<=>t2.current; }
inline auto operator<=>(Timer_t const &t1, i32 t2) { return t1.current<=>t2; }
inline auto operator<=>(i32 t1, Timer_t const &t2) { return t1<=>t2.current; }
inline auto operator<=>(Timer_t const &t1, u32 t2) { return t1.current<=>static_cast<i32>(t2); }
inline auto operator<=>(u32 t1, Timer_t const &t2) { return static_cast<i32>(t1)<=>t2.current; }
inline bool operator!(Timer_t const &t) { return !t.current; }

} // namespace ns

#endif // TIMER_HEADER_INCLUDED
