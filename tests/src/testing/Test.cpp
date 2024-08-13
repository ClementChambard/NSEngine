#include "Test.hpp"

#include <cstdio>

namespace ns::test {

void Test::run() {
  if (fn == nullptr)
    return;
  res = fn();
}

bool Test::print_result(u32 i) {
  if (res == Result::NOT_RAN)
    return false;

  printf("Test %3d: ", i);
  if (res == Result::OK) {
    printf("\033[32mOK     \033[0m");
  } else if (res == Result::SKIPPED) {
    printf("\033[36mSKIPPED\033[0m");
  } else if (res == Result::KO) {
    printf("\033[31mKO     \033[0m");
  }

  if (name) {
    printf(" => %s", name);
  }
  printf("\n");
  return true;
}

bool Test::print_result(cstr pre, u32 i) {
  if (res == Result::NOT_RAN)
    return false;
  printf("%s", pre);
  return print_result(i);
}

} // namespace ns::test
