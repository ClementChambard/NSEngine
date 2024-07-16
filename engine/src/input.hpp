#ifndef INPUT_HEADER_INCLUDED
#define INPUT_HEADER_INCLUDED

#include "./defines.h"
#include "./key.hpp"
#include "./Window.hpp"

namespace ns {

class InputManager {
public:
  static void initialize(usize *memory_requirement, ptr state);
  static void cleanup(ptr state);
  static void update(f64 delta_time);
  static void capture_window_events(Window* w);
};

namespace keyboard {

NS_API bool pressed(Key k);
NS_API bool released(Key k);
NS_API bool down(Key k);
NS_API bool up(Key k);
NS_API bool was_down(Key k);
NS_API bool was_up(Key k);

} // namespace keyboard

namespace mouse {

NS_API bool pressed(Btn b);
NS_API bool released(Btn b);
NS_API bool down(Btn b);
NS_API bool up(Btn b);
NS_API bool was_down(Btn b);
NS_API bool was_up(Btn b);

NS_API void position(i32 &x, i32 &y);
NS_API void prev_position(i32 &x, i32 &y);
NS_API void motion(i32 &x, i32 &y);

NS_API Window* last_window_on();

} // namespace mouse

} // namespace ns

#endif // INPUT_HEADER_INCLUDED
