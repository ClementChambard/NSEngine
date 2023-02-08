#include "NSEngine.h"
#include <cmath>
#include <glm/fwd.hpp>

namespace NSEngine {

    FpsLimiter* engineData::fps = nullptr;
    Camera2D* engineData::cam2d = nullptr;
    Camera3D* engineData::cam3d = nullptr;
    uint32_t engineData::gameflags;
    Window* engineData::NSWindow = nullptr;
    SDL_GLContext engineData::context;
    SDL_Window* engineData::window;
    SDL_Event engineData::event;
    std::vector<SpriteBatch> engineData::layers;
    size_t engineData::targetLayer = 0;
    int engineData::gameHeight, engineData::gameWidth;
    float engineData::displayRatio = 1;
    int engineData::displaymode = 0;
    int engineData::debugLayer;
    std::vector<IEventProcessor*> engineData::eventProcessors;

    void Init()
    {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER) != 0) fatalError("Failed to initialize SDL");

        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        engineData::gameflags = 0b00000001;
        engineData::fps = new FpsLimiter();
        TextureManager::RegisterTexture("assets/engine/textures/defaultTexture.png");

        info("NSEngine Initialized !");
    }

    void Subsystems() {}

    void EndInit()
    {
        engineData::debugLayer = engineData::layers.size();
        addGameLayer(false, true);
    }

    void Quit()
    {
        engineData::gameflags &= 0b11111110;
        engineData::layers.clear();
        if (engineData::cam2d != nullptr) delete engineData::cam2d;
        engineData::cam2d = nullptr;
        if (engineData::cam3d != nullptr) delete engineData::cam3d;
        engineData::cam3d = nullptr;
        delete engineData::fps;
        engineData::window = nullptr;
        //other cleanup
        SDL_Quit();
        info("Quit engine");
    }

    bool IsRunning()
    {
        return engineData::gameflags & 0b00000001;
    }

    void StopRunning()
    {
        engineData::gameflags &= 0b11111110;
    }

    void setMaxFps(int fps)
    {
        engineData::fps->setMaxFps(fps);
    }

    void toggleDebugInfo()
    {
        info("debug info :", engineData::gameflags & 0b00000010 ?"OFF":"ON");
        engineData::gameflags ^= 0b00000010;
    }

    void toggleCulling(bool use)
    {
        if (use)
        {
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
        }
        else glDisable(GL_CULL_FACE);
    }

    void toggleWireframe()
    {
        engineData::gameflags ^= 0b00000100;
        if (engineData::gameflags & 0b00000100)
        {
            info("wireframe : ON");
            glDisable(GL_BLEND);
            glClearColor(.2f, .2f, .2f, 1.f);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else {
            info("wireframe : OFF");
            glEnable(GL_BLEND);
            glClearColor(0.f, 0.f, 0.f, 1.f);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
    }

    void toggleFBF()
    {
        info("frame by frame :", engineData::gameflags & 0b00001000 ? "OFF" : "ON");
        engineData::gameflags ^= 0b00001000;
    }

    void toggle3DCameraControl()
    {
        info("debug camera control :", engineData::gameflags & 0b00010000 ? "OFF" : "ON");
        engineData::gameflags ^= 0b00010000;
    }

    bool IsFBF()
    {
        return engineData::gameflags & 0b00001000;
    }

    void createCamera(int mode, int width, int height)
    {
        if (mode == 0) engineData::cam2d = new Camera2D(width, height);
        if (mode == 1) engineData::cam3d = new Camera3D(width,height);
        info("created new camera of type", mode);
    }

    Camera2D* activeCamera()
    {
        if (engineData::cam2d != nullptr)
            return engineData::cam2d;
        else return nullptr;
    }

    Camera3D* activeCamera3D()
    {
        if (engineData::cam3d != nullptr) return engineData::cam3d;
        else return nullptr;
    }

    glm::mat4 getCameraUniform(bool isstatic)
    {
        if (engineData::cam3d != nullptr)
        {
            return isstatic?engineData::cam3d->getCamStatic():engineData::cam3d->getCam();
        }
        else if (engineData::cam2d != nullptr)
        {
            return isstatic?engineData::cam2d->getCamStatic():engineData::cam2d->getCam();
        }
        return glm::mat4(1);
    }

    glm::mat4 getViewMatrix()
    {
        if (engineData::cam3d != nullptr)
        {
            return engineData::cam3d->getView();
        }
        return glm::mat4(1);
    }
    glm::vec3 camPos()
    {
        if (engineData::cam3d != nullptr)
        {
            return engineData::cam3d->getPosition();
        }
        return glm::vec3(0,0,0);
    }

    void setCamBoundaries(int w, int h)
    {
        if (engineData::cam3d != nullptr) engineData::cam3d->setWH(w,h,(float)w/(float)engineData::gameWidth);
    }

    int addGameLayer(bool depthTest, bool is_static)
    {
        int id = engineData::layers.size();
        engineData::layers.emplace_back();
        engineData::layers.back().is_static = is_static;
        engineData::layers.back().depthTest = depthTest;
        info("added new graphics layer", id, "of type", std::to_string(is_static) + std::to_string(depthTest));
        return id;
    }

    void draw_set_layer(size_t layerID)
    {
        if (layerID >= engineData::layers.size())
        {
            error("no such graphics layer");
            return;
        }
        engineData::targetLayer = layerID;
    }

    void StartFrame()
    {
        engineData::fps->begin();

        InputManager::UpdateKeys();

        //events
        while(SDL_PollEvent(&engineData::event))
        {
            switch (engineData::event.type)
            {
                case SDL_QUIT:
                    engineData::gameflags &= 0b11111110;
                    break;
                default:
                    break;
            }
            InputManager::CheckEvents(engineData::event);

            bool noMouse = false;
            bool noKeyboard = false;
            for (auto e : engineData::eventProcessors)
            {
                e->ProcessEvent(&engineData::event, noKeyboard, noMouse);
            }
        }
        if (Inputs::Keyboard().Pressed(NSK_f1)) NSEngine::toggleDebugInfo();
        if (Inputs::Keyboard().Pressed(NSK_f2)) NSEngine::toggleWireframe();
        if (Inputs::Keyboard().Pressed(NSK_f3)) NSEngine::toggleFBF();
        if (Inputs::Keyboard().Pressed(NSK_f4)) NSEngine::toggle3DCameraControl();
    }

    void EndFrame()
    {
        engineData::fps->end();
        #ifdef NS_USE_INTERPOLATOR
            InterpolateManager::Update_All();
        #endif
    }

    void StartUpdate() {}
    void UpdateEngine(float /*framespeed*/) {}
    void RenderEngine() {}

    void EndUpdate()
    {
        if (activeCamera() != nullptr) activeCamera()->Update();
        if (activeCamera3D() != nullptr) activeCamera3D()->Update(engineData::gameflags & 0b00010000);
    }

    void moveCameraTo(glm::vec3 position)
    {
        if (activeCamera() != nullptr) activeCamera()->setPosition(position);
        else if (activeCamera3D() != nullptr) activeCamera3D()->setPosition(position.x,position.y,position.z);
        else warning("no camera available");
    }

}
