#include <NSEngine.hpp>
#include <cstdio>
#include <input.hpp>

class App : public ns::IEngine {
public:
  App() : ns::IEngine(1280, 720, "Test") {}
  void on_create() override {}
  void on_update() override {
    if (ns::keyboard::pressed(ns::Key::N)) printf("Hello, World!\n");
  }
  void on_render() override {}
  void on_destroy() override {}
};

i32 main() { return App().run(); }
