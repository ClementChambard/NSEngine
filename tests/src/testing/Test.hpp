#ifndef TEST_HEADER_INCLUDED
#define TEST_HEADER_INCLUDED

#include <defines.h>
#include <functional>

namespace ns::test {

struct Test {

  enum class Result {
    OK = 0,
    KO = 1,
    SKIPPED = 2,
    NOT_RAN = 3,
  };

  using FN = std::function<Result(void)>;

  Test() {}
  Test(FN fn) : fn(fn) {}
  Test(cstr name, FN fn) : fn(fn), name(name) {}

  std::function<Result(void)> fn = nullptr;
  cstr name = nullptr;
  Result res = Result::NOT_RAN;

  void run();
  bool print_result(u32 i);
  bool print_result(cstr pre, u32 i);
};

} // namespace ns::test

#endif // TEST_HEADER_INCLUDED
