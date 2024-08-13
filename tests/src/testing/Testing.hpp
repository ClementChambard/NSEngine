#ifndef TESTING_HEADER_INCLUDED
#define TESTING_HEADER_INCLUDED

#include "./Setup.hpp"
#include "./TestGroup.hpp"
#include <cstdio>

////////////////////////////////////////////////////////////////////////////////
// Batch creation                                                             //
//                                                                            //
#define TEST_BATCH_BEGIN(name, s_name)                                         \
  void name() {                                                                \
    ns::test::TestGroup group(s_name); (void)(1

#define TEST(s_name) ); group.test(s_name, []()

#define TEST_BATCH_END );                                                      \
  group.run();                                                                 \
  }
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Results                                                                    //
//                                                                            //
#define TEST_OK return ns::test::Test::Result::OK;
#define TEST_KO return ns::test::Test::Result::KO;
#define SKIP_TEST return ns::test::Test::Result::SKIPPED;
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Utils                                                                      //
//                                                                            //
#define FAIL_TEST(tststr)                                                      \
  {                                                                            \
    printf("Test failed at %s:%d: " tststr "\n", __FILE__, __LINE__);          \
    TEST_KO                                                                    \
  }
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Tests                                                                      //
//                                                                            //
#define TEST_EQ(expr1, expr2)                                                  \
  {                                                                            \
    if (expr1 != expr2)                                                        \
      FAIL_TEST(#expr1 " == " #expr2)                                          \
  }

#define TEST_NE(expr1, expr2)                                                  \
  {                                                                            \
    if (expr1 == expr2)                                                        \
      FAIL_TEST(#expr1 " != " #expr2)                                          \
  }

#define TEST_TRUE(expr1) TEST_EQ(expr1, true)
#define TEST_FALSE(expr1) TEST_EQ(expr1, false)
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#endif // TESTING_HEADER_INCLUDED
