#include "NSEngine.hpp"
#include "input.hpp"
#include "./logger.h"
#include "./memory.h"
#include <cstdio>
#include "platform/platform.h"

#include <GL/glew.h>

#include <cstdlib> // for exit

namespace ns {

IEngine* IEngine::instance = nullptr;

static usize log_req = 0;
static ptr log_mem = 0;
static usize im_req = 0;
static ptr im_mem = 0;

extern void NSMOD_initModules();
extern void NSMOD_shutdownModules();
extern void NSMOD_updateModules();


void opengl_debug_callback(unsigned int source, unsigned int type, unsigned int id, unsigned int severity, int, const char* message, const void*) {

    const char* src_str;
    switch (source) {
        case GL_DEBUG_SOURCE_API: src_str = " [API]"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM: src_str = " [WINDOW SYSTEM]"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: src_str = " [SHADER COMPILER]"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY: src_str = " [THIRD PARTY]"; break;
        case GL_DEBUG_SOURCE_APPLICATION: src_str = " [APPLICATION]"; break;
        case GL_DEBUG_SOURCE_OTHER: src_str = ""; break;
        default: std::printf("INVALID SOURCE: %d\n", source); return;
    }

    const char* type_str;
    switch (type) {
    case GL_DEBUG_TYPE_ERROR: type_str = " [ERROR]"; break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: type_str = " [DEPRECATED BEHAVIOR]"; break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: type_str = " [UNDEFINED BEHAVIOR]"; break;
    case GL_DEBUG_TYPE_PORTABILITY: type_str = " [PORTABILITY]"; break;
    case GL_DEBUG_TYPE_PERFORMANCE: type_str = " [PERFORMANCE]"; break;
    case GL_DEBUG_TYPE_MARKER: type_str = " [MARKER]"; break;
    case GL_DEBUG_TYPE_PUSH_GROUP: type_str = " [PUSH GROUP]"; break;
    case GL_DEBUG_TYPE_POP_GROUP: type_str = " [POP GROUP]"; break;
    case GL_DEBUG_TYPE_OTHER: type_str = ""; break;
    default: std::printf("INVALID TYPE: %d\n", type); return;
    }

    const char* sev_str;
    switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH: sev_str = "\x1b[31m"; break;
    case GL_DEBUG_SEVERITY_MEDIUM: sev_str = ""; break;
    case GL_DEBUG_SEVERITY_LOW: sev_str = ""; break;
    case GL_DEBUG_SEVERITY_NOTIFICATION: return; // sev_str = ""; break;
    default: std::printf("INVALID SEVERITY: %d\n", severity); return;
    }

    std::printf("\x1b[1m[OPENGL: %d]\x1b[0m%s%s%s: %s\x1b[0m\n", id, src_str, sev_str, type_str, message);
}



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

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(opengl_debug_callback, nullptr);

    NSMOD_initModules();

    byte data[] = {255, 255, 255, 255};
    m_defaultTexture = construct<Texture, MemTag::TEXTURE>(1, 1, data);

    m_gameflags.val = 1;
    NS_INFO("NSEngine Initialized !");
}

IEngine::~IEngine()
{

    if (m_mainWindow.is_open()) m_mainWindow.close();

    NS_INFO("Quit engine");

    ns::destroy<MemTag::TEXTURE>(m_defaultTexture);

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

    NSMOD_updateModules();

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

    NSMOD_shutdownModules();
}

}
