#include "Engine.hpp"
#include "NSEngine.h"

namespace NSEngine {

IEngine::IEngine(int w, int h, std::string name)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER) != 0) fatalError("Failed to initialize SDL");

    m_window.Init(name.c_str(), w, h, 0);

    TextureManager::RegisterTexture("assets/engine/textures/defaultTexture.png");

    m_running = true;
    info("NSEngine Initialized !");

    // remove
    engineData::fps = new FpsLimiter();
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
    while (m_running) {
        m_fps.begin();
        on_update_engine();
        on_render_engine();
            InterpolateManager::Update_All();
        m_fps.end();
    }
    on_destroy_engine();
}

void IEngine::on_create_engine()
{
    on_create();
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
                    engineData::gameflags &= 0b11111110;
                    m_running = false;
                    break;
                default:
                    break;
            }
            InputManager::CheckEvents(m_event);

            bool noMouse = false;
            bool noKeyboard = false;
            for (auto e : engineData::eventProcessors)
            {
                e->ProcessEvent(&m_event, noKeyboard, noMouse);
            }
        }

    //Update debug commands
    if (Inputs::Keyboard().Pressed(NSK_f1)) toggleDebugInfo();
    if (Inputs::Keyboard().Pressed(NSK_f2)) toggleWireframe();
    if (Inputs::Keyboard().Pressed(NSK_f3)) toggleFBF();
    if (Inputs::Keyboard().Pressed(NSK_f4)) toggle3DCameraControl();
    if (Inputs::Keyboard().Pressed(NSK_f10)) m_window.nextDisplaymode();
    if (NSEngine::IsFBF() && !Inputs::Keyboard().Pressed(NSK_backspace) && !Inputs::Keyboard().Down(NSK_equals)) return;

    on_update();

    //Update cameras
    if (activeCamera() != nullptr) activeCamera()->Update();
    if (activeCamera3D() != nullptr) activeCamera3D()->Update(engineData::gameflags & 0b00010000);
}

void IEngine::on_render_engine()
{
    m_window.InitDrawing();
    on_render();
    m_window.EndDrawing();
}

void IEngine::on_destroy_engine()
{
    on_destroy();
    engineData::layers.clear();
    if (engineData::cam2d != nullptr) delete engineData::cam2d;
    engineData::cam2d = nullptr;
    if (engineData::cam3d != nullptr) delete engineData::cam3d;
    engineData::cam3d = nullptr;
    // remove
    delete engineData::fps;
}

}
