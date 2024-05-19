#ifndef TIMER_INCLUDED_H
#define TIMER_INCLUDED_H

#include "./defines.h"

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

  Timer_t &operator=(i32 c) {
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

  i32 previous = -1;
  i32 current = 0;
  f32 current_f = 0.f;
  // f32* __game_speed__disused = nullptr;
  // i32 control = 0;
};

inline bool operator==(Timer_t const &t1, Timer_t const &t2) {
  return t1.current == t2.current;
}
inline bool operator==(Timer_t const &t1, i32 t2) {
  return t1.current == t2;
}
inline bool operator==(i32 t1, Timer_t const &t2) { return t2 == t1; }
inline bool operator!=(Timer_t const &t1, Timer_t const &t2) {
  return t1.current != t2.current;
}
inline bool operator!=(Timer_t const &t1, i32 t2) {
  return t1.current != t2;
}
inline bool operator!=(i32 t1, Timer_t const &t2) { return t2 != t1; }
inline bool operator<=(Timer_t const &t1, Timer_t const &t2) {
  return t1.current <= t2.current;
}
inline bool operator<=(Timer_t const &t1, i32 t2) {
  return t1.current <= t2;
}
inline bool operator<=(i32 t1, Timer_t const &t2) {
  return t1 <= t2.current;
}
inline bool operator>=(Timer_t const &t1, Timer_t const &t2) {
  return t1.current >= t2.current;
}
inline bool operator>=(Timer_t const &t1, i32 t2) {
  return t1.current >= t2;
}
inline bool operator>=(i32 t1, Timer_t const &t2) {
  return t1 >= t2.current;
}
inline bool operator<(Timer_t const &t1, Timer_t const &t2) {
  return t1.current < t2.current;
}
inline bool operator<(Timer_t const &t1, i32 t2) { return t1.current < t2; }
inline bool operator<(i32 t1, Timer_t const &t2) { return t1 < t2.current; }
inline bool operator>(Timer_t const &t1, Timer_t const &t2) {
  return t1.current > t2.current;
}
inline bool operator>(Timer_t const &t1, i32 t2) { return t1.current > t2; }
inline bool operator>(i32 t1, Timer_t const &t2) { return t1 > t2.current; }
inline bool operator!(Timer_t const &t) { return !t.current; }

}  // namespace NSEngine

#endif // !TIMER_INCLUDED_H
