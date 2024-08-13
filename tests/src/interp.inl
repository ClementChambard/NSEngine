#include "testing/Testing.hpp"
#include <Interp.hpp>

TEST_BATCH_BEGIN(run_interp_tests, "interp")

TEST("LINEAR") {
  auto func = ns::getInterpFunc(ns::InterpMethod::LINEAR);
  TEST_EQ(func(0), 0);
  TEST_EQ(func(0.25), 0.25);
  TEST_EQ(func(0.5), 0.5);
  TEST_EQ(func(0.75), 0.75);
  TEST_EQ(func(1), 1);
  TEST_OK;
}

TEST("EASEIN2") {
  auto func = ns::getInterpFunc(ns::InterpMethod::EASEIN2);
  TEST_EQ(func(0), 0);
  TEST_EQ(func(0.25), 0.0625);
  TEST_EQ(func(0.5), 0.25);
  TEST_EQ(func(0.75), 0.5625);
  TEST_EQ(func(1), 1);
  TEST_OK;
}

TEST("EASEIN3") {
  auto func = ns::getInterpFunc(ns::InterpMethod::EASEIN3);
  TEST_EQ(func(0), 0);
  TEST_EQ(func(0.25), 0.015625);
  TEST_EQ(func(0.5), 0.125);
  TEST_EQ(func(0.75), 0.421875);
  TEST_EQ(func(1), 1);
  TEST_OK;
}

TEST("EASEIN4") {
  auto func = ns::getInterpFunc(ns::InterpMethod::EASEIN4);
  TEST_EQ(func(0), 0);
  TEST_EQ(func(0.25), 0.00390625);
  TEST_EQ(func(0.5), 0.0625);
  TEST_EQ(func(0.75), 0.31640625);
  TEST_EQ(func(1), 1);
  TEST_OK;
}

TEST("EASEOUT2") {
  auto func = ns::getInterpFunc(ns::InterpMethod::EASEOUT2);
  TEST_EQ(func(0), 0);
  TEST_EQ(func(0.25), 0.4375);
  TEST_EQ(func(0.5), 0.75);
  TEST_EQ(func(0.75), 0.9375);
  TEST_EQ(func(1), 1);
  TEST_OK;
}

TEST("EASEOUT3") {
  auto func = ns::getInterpFunc(ns::InterpMethod::EASEOUT3);
  TEST_EQ(func(0), 0);
  TEST_EQ(func(0.25), 0.578125);
  TEST_EQ(func(0.5), 0.875);
  TEST_EQ(func(0.75), 0.984375);
  TEST_EQ(func(1), 1);
  TEST_OK;
}

TEST("EASEOUT4") {
  auto func = ns::getInterpFunc(ns::InterpMethod::EASEOUT4);
  TEST_EQ(func(0), 0);
  TEST_EQ(func(0.25), 0.68359375);
  TEST_EQ(func(0.5), 0.9375);
  TEST_EQ(func(0.75), 0.99609375);
  TEST_EQ(func(1), 1);
  TEST_OK;
}

TEST("EASEINOUT2") {
  auto func = ns::getInterpFunc(ns::InterpMethod::EASEINOUT2);
  TEST_EQ(func(0), 0);
  TEST_EQ(func(0.25), 0.125);
  TEST_EQ(func(0.5), 0.5);
  TEST_EQ(func(0.75), 0.875);
  TEST_EQ(func(1), 1);
  TEST_OK;
}

TEST("EASEINOUT3") {
  auto func = ns::getInterpFunc(ns::InterpMethod::EASEINOUT3);
  TEST_EQ(func(0), 0);
  TEST_EQ(func(0.25), 0.0625);
  TEST_EQ(func(0.5), 0.5);
  TEST_EQ(func(0.75), 0.9375);
  TEST_EQ(func(1), 1);
  TEST_OK;
}

TEST("EASEINOUT4") {
  auto func = ns::getInterpFunc(ns::InterpMethod::EASEINOUT4);
  TEST_EQ(func(0), 0);
  TEST_EQ(func(0.25), 0.03125);
  TEST_EQ(func(0.5), 0.5);
  TEST_EQ(func(0.75), 0.96875);
  TEST_EQ(func(1), 1);
  TEST_OK;
}

TEST("EASEOUTIN2") {
  auto func = ns::getInterpFunc(ns::InterpMethod::EASEOUTIN2);
  TEST_EQ(func(0), 0);
  TEST_EQ(func(0.25), 0.375);
  TEST_EQ(func(0.5), 0.5);
  TEST_EQ(func(0.75), 0.625);
  TEST_EQ(func(1), 1);
  TEST_OK;
}

TEST("EASEOUTIN3") {
  auto func = ns::getInterpFunc(ns::InterpMethod::EASEOUTIN3);
  TEST_EQ(func(0), 0);
  TEST_EQ(func(0.25), 0.4375);
  TEST_EQ(func(0.5), 0.5);
  TEST_EQ(func(0.75), 0.5625);
  TEST_EQ(func(1), 1);
  TEST_OK;
}

TEST("EASEOUTIN4") {
  auto func = ns::getInterpFunc(ns::InterpMethod::EASEOUTIN4);
  TEST_EQ(func(0), 0);
  TEST_EQ(func(0.25), 0.46875);
  TEST_EQ(func(0.5), 0.5);
  TEST_EQ(func(0.75), 0.53125);
  TEST_EQ(func(1), 1);
  TEST_OK;
}

TEST("_15") {
  auto func = ns::getInterpFunc(ns::InterpMethod::_15);
  TEST_EQ(func(0), 0);
  TEST_EQ(func(0.25), 0);
  TEST_EQ(func(0.5), 0);
  TEST_EQ(func(0.75), 0);
  TEST_EQ(func(1), 1);
  TEST_OK;
}

TEST("_16") {
  auto func = ns::getInterpFunc(ns::InterpMethod::_16);
  TEST_EQ(func(0), 0);
  TEST_EQ(func(0.25), 1);
  TEST_EQ(func(0.5), 1);
  TEST_EQ(func(0.75), 1);
  TEST_EQ(func(1), 1);
  TEST_OK;
}

// TEST("Interpolator 1") {
//   auto interp = ns::Interp<float>();
//   interp.start(0, 1, 4, 0);
//   TEST_EQ(interp.step(), 0.25);
//   TEST_EQ(interp.step(), 0.5);
//   TEST_EQ(interp.step(), 0.75);
//   TEST_EQ(interp.step(), 1.0);
//   TEST_EQ(interp.end_time, 0);
//   TEST_OK;
// }

TEST_BATCH_END
