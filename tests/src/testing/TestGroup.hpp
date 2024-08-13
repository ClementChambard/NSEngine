#ifndef TESTGROUP_HEADER_INCLUDED
#define TESTGROUP_HEADER_INCLUDED

#include "./Test.hpp"
#include <initializer_list>
#include <vector>

namespace ns::test {

struct TestGroup {
  TestGroup() = default;
  TestGroup(cstr name) : name(name) {}
  TestGroup(std::initializer_list<Test> tests);

  std::vector<Test> tests;
  cstr name = nullptr;

  void run();

  template <typename... Args> TestGroup &test(Args... args) {
    tests.emplace_back(args...);
    return *this;
  }
};

} // namespace ns::test

#endif // TESTGROUP_HEADER_INCLUDED
