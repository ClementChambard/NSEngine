#ifndef NSENGINE_HEADER_INCLUDED
#define NSENGINE_HEADER_INCLUDED

#include "./defines.h"
#include "./logger.h"
#include "SpriteBatch.h"
#include "Window.hpp"

namespace ns {

class IEngine {
public:
  IEngine(u32 width, u32 height, cstr name);
  virtual ~IEngine();
  IEngine(IEngine const &) = delete;
  IEngine(IEngine &&) = delete;
  IEngine &operator=(IEngine const &) = delete;
  IEngine &operator=(IEngine &&) = delete;

  /**
   * user defined create event
   */
  virtual void on_create() { NS_WARN("default on_create called"); }

  /**
   * user defined update event
   */
  virtual void on_update() { NS_WARN("default on_update called"); }

  /**
   * user defined render event
   */
  virtual void on_render() { NS_WARN("default on_render called"); }

  /**
   * user defined destroy event
   */
  virtual void on_destroy() { NS_WARN("default on_destroy called"); }

  /**
   * Run the app
   */
  i32 run();

  /**
   * Stops the engine
   */
  void stop() { m_gameflags.running = false; }

  /**
   * Changes the maximum amount of frame per seconds
   */
  void set_max_fps(u32 fps) { 
    NS_INFO("max FPS set to %u", fps);
    m_maxFps = fps;
  }

  /**
   * Instance of the game engine
   */
  static IEngine *instance;

  /**
   * Getter for the game flags (as reference so possible to change it here)
   */
  auto &flags() { return m_gameflags; }

  f32 fps() const { return m_currentFps; }
  f32 game_speed() const { return m_gameSpeed; }

  void set_game_speed(f32 gs) { m_gameSpeed = gs; }

  i32 add_game_layer();

private:
  void on_create_engine();
  void on_update_engine();
  void on_render_engine();
  void on_destroy_engine();

  Window m_mainWindow;

  f32 m_gameSpeed = 1.0f;

  u32 m_maxFps = 60;
  f32 m_currentFps = 0.f;
  f32 m_lastFrameTime = 0.f;

  union {
    u32 val;
    struct {
      u32 running : 1;
      u32 debugInfo : 1;
      u32 wireframe : 1;
      u32 framebyframe : 1;
      u32 freecam : 1;
    };
  } m_gameflags;

  // Camera3D *m_cam3d = nullptr;

  std::vector<SpriteBatch> m_layers;

  friend std::vector<SpriteBatch> &get_game_layers();

  friend Window *get_main_window();
};

inline IEngine *get_instance() { return IEngine::instance; }

inline Window *get_main_window() { return &IEngine::instance->m_mainWindow; }

std::vector<SpriteBatch> &get_game_layers();

Texture *get_default_texture();

extern i32 DEBUG_LAYER_ID;

} // namespace ns

#endif // NSENGINE_HEADER_INCLUDED
