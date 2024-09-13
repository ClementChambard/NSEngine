#include <NSEngine.hpp>
#include <input.hpp>
#include <NSAudio.h>

int snd;
int src;

class App : public ns::IEngine {
public:
  App() : ns::IEngine(1280, 720, "Test") {}
  void on_create() override {
    src = ns::audio::create_source();
    snd = ns::audio::add_sfx("test.wav");
  }
  void on_update() override {
    if (ns::keyboard::pressed(ns::Key::P)) {
      ns::audio::play_sound_at_source(src, snd);
    }
  }
  void on_render() override {}
  void on_destroy() override {}
};

i32 main() { return App().run(); }
