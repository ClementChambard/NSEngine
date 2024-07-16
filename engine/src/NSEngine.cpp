#include "NSEngine.hpp"
#include "input.hpp"
#include "TextureManager.h"
#include "./logger.h"
#include "./memory.h"
#include <cstdio>
#include "platform/platform.h"

#include <GL/glew.h>

#include <cstdlib> // for exit

namespace ns {

i32 DEBUG_LAYER_ID = 0;

IEngine* IEngine::instance = nullptr;

static usize log_req = 0;
static ptr log_mem = 0;
static usize im_req = 0;
static ptr im_mem = 0;

extern void NSMOD_initModules();

IEngine::IEngine(u32 w, u32 h, cstr name)
{
    if (instance) {
        NS_FATAL("Can't have multiple instances of Engine");
        exit(1);
    }
    instance = this;

    memory_system_initialize({});
    initialize_logging(&log_req, log_mem);
    log_mem = ns::alloc_raw(log_req, MemTag::APPLICATION);
    initialize_logging(&log_req, log_mem);
    ns::InputManager::initialize(&im_req, im_mem);
    im_mem = ns::alloc_raw(im_req, MemTag::APPLICATION);
    ns::InputManager::initialize(&im_req, im_mem);

    m_mainWindow.configure({name, w, h, 0});
    m_mainWindow.open();
    ns::InputManager::capture_window_events(&m_mainWindow);
    NS_INFO("Main window successfuly initialized");

    GLenum Error = glewInit();
    if (Error != GLEW_OK) {
      NS_FATAL("Failed to initialize GLEW: %s", glewGetErrorString(Error));
      exit(1);
    }

    NSMOD_initModules();

    TextureManager::register_texture("assets/engine/textures/defaultTexture.png");

    m_gameflags.val = 1;
    NS_INFO("NSEngine Initialized !");
}

IEngine::~IEngine()
{
#ifdef NS_USE_IMGUI
    delete ImGuiContext::getInstance();
#endif

    if (m_mainWindow.is_open()) m_mainWindow.close();

    NS_INFO("Quit engine");

    ns::InputManager::cleanup(im_mem);
    ns::free_raw(im_mem, im_req, MemTag::APPLICATION);
    shutdown_logging(log_mem);
    ns::free_raw(log_mem, log_req, MemTag::APPLICATION);

    pstr s = get_memory_usage_str();
    printf("\033[0m%s\n", s);
    ns::free_raw(s, 0, MemTag::UNTRACKED);
    memory_system_shutdown();

    instance = nullptr;
}

i32 IEngine::run()
{
    on_create_engine();

    // TODO: Should be able to update this value when updating m_maxFps
    f64 target_frame_time = 1.0 / static_cast<f64>(m_maxFps);

    while (m_gameflags.running) {
        f64 frame_start_time = platform::get_absolute_time();

        // Game loop
        on_update_engine();
        if (!m_gameflags.running) break;
        on_render_engine();
        
        // Post game loop
        f64 frame_end_time = platform::get_absolute_time();
        f64 frame_elapsed_time = frame_end_time - frame_start_time;
        m_lastFrameTime = static_cast<f32>(frame_elapsed_time);
        f64 remaining_seconds = target_frame_time - frame_elapsed_time;

        // TODO: Better fps calculations
        if (remaining_seconds > 0.0) {
            m_currentFps = m_maxFps;
            u64 remaining_ms = static_cast<u64>(remaining_seconds * 1000);
            if (remaining_ms > 0) platform::sleep(remaining_ms - 1);
        } else {
            m_currentFps = 1.0 / frame_elapsed_time;
        }
    }
    on_destroy_engine();
    // TODO: return exitcode ?
    return 0;
}

void IEngine::on_create_engine()
{
    on_create();
    DEBUG_LAYER_ID = add_game_layer();
}

void IEngine::on_update_engine()
{
    ns::InputManager::update(m_lastFrameTime);

    //Process events
    if (m_mainWindow.is_open()) m_mainWindow.process_events();
    if (!m_mainWindow.is_open()) { m_gameflags.running = false; return; }

    //Update debug commands
    if (keyboard::pressed(Key::F1)) m_gameflags.debugInfo = !m_gameflags.debugInfo;
    if (keyboard::pressed(Key::F2)) m_gameflags.wireframe = !m_gameflags.wireframe;
    if (keyboard::pressed(Key::F3)) m_gameflags.framebyframe = !m_gameflags.framebyframe;
    if (keyboard::pressed(Key::F4)) m_gameflags.freecam = !m_gameflags.freecam;
    if (m_gameflags.framebyframe && !keyboard::pressed(Key::BACKSPACE) && !keyboard::down(Key::NUMPAD_EQUAL)) return;

    on_update();
}

void IEngine::on_render_engine()
{
    on_render();
    m_mainWindow.swap_buffers();
}

void IEngine::on_destroy_engine()
{
    on_destroy();
    m_layers.clear();
}

i32 IEngine::add_game_layer() {
  i32 id = m_layers.size();
  m_layers.emplace_back();
  NS_INFO("added new graphics layer %d", id);
  return id;
}

std::vector<SpriteBatch>& get_game_layers() {
    return IEngine::instance->m_layers;
}

Texture *get_default_texture() {
    return TextureManager::get_texture(1);
}

}
