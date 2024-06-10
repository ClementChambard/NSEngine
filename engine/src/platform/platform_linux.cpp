#include "./platform.h"

#if NS_PLATFORM_LINUX

#include "../logger.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#if _POSIX_C_SOURCE >= 199309L
#include <time.h>
#else
#include <unistd.h>
#endif

// #define TRACK_PLATFORM_ALLOCATIONS

namespace ns::platform {

ptr allocate_memory(usize size, bool /* aligned */) {
#ifdef TRACK_PLATFORM_ALLOCATIONS
  NS_TRACE("Platform allocation of size %lluB", size);
#endif
  return std::malloc(size);
}

ptr reallocate_memory(ptr block, usize new_size, bool /* aligned */) {
#ifdef TRACK_PLATFORM_ALLOCATIONS
  NS_TRACE("Platform reallocation of size %lluB", new_size);
#endif
  return std::realloc(block, new_size);
}

void free_memory(ptr block, bool /* aligned */) {
#ifdef TRACK_PLATFORM_ALLOCATIONS
  NS_TRACE("Platform free");
#endif
  std::free(block);
}

ptr zero_memory(ptr block, usize size) { return std::memset(block, 0, size); }

ptr copy_memory(ptr dest, roptr source, usize size) {
  return std::memcpy(dest, source, size);
}

ptr set_memory(ptr dest, i32 value, usize size) {
  return std::memset(dest, value, size);
}

void console_write(cstr message, u8 color) {
  cstr levels[6] = {"0;41", "1;31", "1;33", "1;32", "1;34", "0;36"};
  std::printf("\033[%sm%s\033[0m", levels[color], message);
}

void console_write_error(cstr message, u8 color) {
  cstr levels[6] = {"0;41", "1;31", "1;33", "1;32", "1;34", "0;36"};
  std::printf("\033[%sm%s\033[0m", levels[color], message);
}

f64 get_absolute_time() {
  timespec now;
  clock_gettime(CLOCK_MONOTONIC, &now);
  return now.tv_sec + now.tv_nsec * 0.000000001;
}

void sleep(u64 ms) {
#if _POSIX_C_SOURCE >= 199309L
  timespec ts;
  ts.tv_sec = ms / 1000;
  ts.tv_nsec = (ms % 1000) * 1000 * 1000;
  nanosleep(&ts, 0);
#else
  if (ms >= 1000) {
    sleep(ms / 1000);
  }
  usleep((ms % 1000) * 1000);
#endif
}

} // namespace ns::platform

#endif // NS_PLATFORM_LINUX
