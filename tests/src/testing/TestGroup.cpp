#include "./TestGroup.hpp"
#include <cstdio>

namespace ns::test {

TestGroup::TestGroup(std::initializer_list<Test> tests) { this->tests = tests; }

void TestGroup::run() {
  if (name)
    printf("Running test group \"%s\"...\n", name);
  else
    printf("Running tests...\n");
  i32 i = 0;
  i32 ko = 0;
  i32 ok = 0;
  i32 skipped = 0;
  for (auto &t : tests) {
    t.run();
    i += t.print_result("  ", i);
    if (t.res == Test::Result::OK)
      ok++;
    if (t.res == Test::Result::KO)
      ko++;
    if (t.res == Test::Result::SKIPPED)
      skipped++;
  }
  printf("Results: %3d \033[32mOK\033[0m, %3d \033[31mKO\033[0m, %3d "
         "\033[36mSKIPPED\033[0m\n",
         ok, ko, skipped);
}

} // namespace ns::test
