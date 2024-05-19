#include "NSEngine.hpp"
#include <SDL2/SDL.h>
#include "InputManager.h"
#include "TextureManager.h"

namespace ns {

i32 DEBUG_LAYER_ID = 0;

IEngine* IEngine::instance = nullptr;

IEngine::IEngine(i32 w, i32 h, cstr name)
{
    if (instance) fatalError("Can't have multiple instances of Engine");

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER) != 0) fatalError("Failed to initialize SDL");

    m_window.Init(name, w, h, 0);

    TextureManager::RegisterTexture("assets/engine/textures/defaultTexture.png");

    m_gameflags.val = 1;
    info("NSEngine Initialized !");

    instance = this;
}

IEngine::~IEngine()
{
    m_window.destroy();
    SDL_Quit();
    info("Quit engine");
}

void IEngine::run()
{
    on_create_engine();
    while (m_gameflags.flags.running) {
        m_fps.begin();
        on_update_engine();
        on_render_engine();
        m_fps.end();
    }
    on_destroy_engine();
}

void IEngine::on_create_engine()
{
    on_create();
    DEBUG_LAYER_ID = addGameLayer(false, true);
}

void IEngine::on_update_engine()
{
    InputManager::UpdateKeys();

        //Process events
        while(SDL_PollEvent(&m_event))
        {
            switch (m_event.type)
            {
                case SDL_QUIT:
                    m_gameflags.flags.running = false;
                    break;
                default:
                    break;
            }
            InputManager::CheckEvents(m_event);

            bool noMouse = false;
            bool noKeyboard = false;
            for (auto e : m_eventProcessors)
            {
                if (m_event.type == SDL_MOUSEMOTION) noMouse = false;
                e->ProcessEvent(&m_event, noKeyboard, noMouse);
            }
        }

    //Update debug commands
    if (Inputs::Keyboard().Pressed(NSK_f1)) m_gameflags.flags.debugInfo = !m_gameflags.flags.debugInfo;
    if (Inputs::Keyboard().Pressed(NSK_f2)) m_gameflags.flags.wireframe = !m_gameflags.flags.wireframe;
    if (Inputs::Keyboard().Pressed(NSK_f3)) m_gameflags.flags.framebyframe = !m_gameflags.flags.framebyframe;
    if (Inputs::Keyboard().Pressed(NSK_f4)) m_gameflags.flags.freecam = !m_gameflags.flags.freecam;
    if (Inputs::Keyboard().Pressed(NSK_f10)) m_window.nextDisplaymode();
    if (m_gameflags.flags.framebyframe && !Inputs::Keyboard().Pressed(NSK_backspace) && !Inputs::Keyboard().Down(NSK_equals)) return;

    on_update();

    //Update camera
    if (m_cam3d) m_cam3d->Update(m_gameflags.flags.freecam);
}

void IEngine::on_render_engine()
{
    on_render_pre();
    m_window.InitDrawing();
    on_render();
    m_window.EndDrawing(render_func);
    on_render_post();
    SDL_GL_SwapWindow(m_window.getSdlWindow());
}

void IEngine::on_destroy_engine()
{
    on_destroy();
    m_layers.clear();
    m_eventProcessors.clear();
    if (m_cam3d != nullptr) delete m_cam3d;
    m_cam3d = nullptr;
}

i32 IEngine::addGameLayer(bool depthTest, bool is_static) {
  i32 id = m_layers.size();
  m_layers.emplace_back();
  m_layers.back().is_static = is_static;
  m_layers.back().depthTest = depthTest;
  info("added new graphics layer", id, "of type", is_static, depthTest);
  return id;
}

Camera3D* activeCamera3D() {
    return IEngine::instance->m_cam3d;
}

std::vector<SpriteBatch>& getGameLayers() {
    return IEngine::instance->m_layers;
}

}
