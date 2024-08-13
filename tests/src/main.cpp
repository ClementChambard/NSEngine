#include "interp.inl"
#include "memory.inl"
#include "testing.inl"
#include "testing/Testing.hpp"

int main() {
  ns::test::init_testing();
  // run_testing_tests();
  run_memory_tests();
  run_interp_tests();
  ns::test::shutdown_testing();
  return 0;
}
