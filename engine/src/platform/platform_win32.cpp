#include "./platform.h"

#if NS_PLATFORM_WINDOWS

#include "../core/logger.h"

#include <stdlib.h>

namespace ns::platform {

static f64 clock_frequency;
static LARGE_INTEGER start_time;

void clock_setup() {
  LARGE_INTEGER frequency;
  QueryPerformanceFrequency(&frequency);
  clock_frequency = 1.0 / static_cast<f64>(frequency.QuadPart);
  QueryPerformanceCounter(&start_time);
}

ptr allocate_memory(usize size, bool /* aligned */) {
  return std::malloc(size);
}

ptr reallocate_memory(ptr block, usize new_size, bool /* aligned */) {
  return std::realloc(block, new_size);
}

void free_memory(ptr block, bool /* aligned */) { std::free(block); }

ptr zero_memory(ptr block, usize size) { return memset(block, 0, size); }

ptr copy_memory(ptr dest, roptr source, usize size) {
  return memcpy(dest, source, size);
}

ptr set_memory(ptr dest, i32 value, usize size) {
  return memset(dest, value, size);
}

void console_write(cstr message, u8 color) {
  HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
  static u8 levels[6] = {64, 4, 6, 2, 1, 8};
  SetConsoleTextAttribute(console_handle, levels[color]);

  OutputDebugStringA(message);
  DWORD length = reinterpret_cast<DWORD>(strlen(message));
  LPDWORD number_written = 0;
  WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), message, length,
                number_written, 0);
}

void console_write_error(cstr message, u8 color) {
  HANDLE console_handle = GetStdHandle(STD_ERROR_HANDLE);
  static u8 levels[6] = {64, 4, 6, 2, 1, 8};
  SetConsoleTextAttribute(console_handle, levels[color]);

  OutputDebugStringA(message);
  DWORD length = reinterpret_cast<DWORD>(strlen(message));
  LPDWORD number_written = 0;
  WriteConsoleA(GetStdHandle(STD_ERROR_HANDLE), message, length, number_written,
                0);
}

f64 get_absolute_time() {
  if (!clock_frequency)
    clock_setup();
  LARGE_INTEGER now_time;
  QueryPerformanceCounter(&now_time);
  return static_cast<f64>(now_time.QuadPart) * clock_frequency;
}

void sleep(u64 ms) { Sleep(ms); }

} // namespace ns::platform

#endif // NS_PLATFORM_WINDOWS
