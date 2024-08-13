#include "Setup.hpp"

#include <memory.h>

namespace ns::test {

void init_testing() { memory_system_initialize({}); }

void shutdown_testing() { memory_system_shutdown(); }

} // namespace ns::test
