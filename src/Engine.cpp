#include "Engine.hpp"
#include "NSEngine.h"
#include <SDL2/SDL.h>

namespace NSEngine {

IEngine* IEngine::instance = nullptr;

IEngine::IEngine(int w, int h, std::string name)
{
    if (instance) fatalError("Can't have multiple instances of Engine");

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER) != 0) fatalError("Failed to initialize SDL");

    m_window.Init(name.c_str(), w, h, 0);

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
    //Remove layers
    engineData::debugLayer = engineData::layers.size();
    addGameLayer(false, true);
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
            for (auto e : engineData::eventProcessors)
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

    //Update cameras
    if (activeCamera() != nullptr) activeCamera()->Update();
    if (activeCamera3D() != nullptr) activeCamera3D()->Update(m_gameflags.flags.freecam);

    //Remove
    InterpolateManager::Update_All();
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
    engineData::layers.clear();
    if (engineData::cam2d != nullptr) delete engineData::cam2d;
    engineData::cam2d = nullptr;
    if (engineData::cam3d != nullptr) delete engineData::cam3d;
    engineData::cam3d = nullptr;
}

}
