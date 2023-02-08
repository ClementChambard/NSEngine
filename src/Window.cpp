#include "Window.h"
#include "Error.h"
#include "NSEngine.h"
#include <glm/fwd.hpp>

namespace NSEngine {

    Window::Window() {}
    Window::~Window() {}


    void Window::Init(const char* name, int width, int height, int flag)
    {
        engineData::NSWindow = this;
        engineData::gameWidth = windowWidth = width;
        engineData::gameHeight = windowHeight = height;
        windowFlags = flag | SDL_WINDOW_OPENGL;

        addDisplayMode({width, height, (flag & SDL_WINDOW_FULLSCREEN) != 0});

        engineData::window = m_window = SDL_CreateWindow(name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, windowFlags);
        if (m_window == nullptr) fatalError("Failed to create window");

        engineData::context = m_context = SDL_GL_CreateContext(engineData::window);
        if (m_context == nullptr) fatalError("Failed to create GL context");

        GLenum Error = glewInit();
        if (Error != GLEW_OK) fatalError("Failed to initialize GLEW");

        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetSwapInterval(1);

        glClearColor(0.f, 0.f, 0.f, 1.f);
        glEnable(GL_MULTISAMPLE);
        
        baseShader = new AnimShader();

        info("Window successfuly initialized");

    }

    void Window::destroy() 
    {
        SDL_GL_DeleteContext(engineData::context);
        SDL_DestroyWindow(engineData::window);
    }

    int Window::addDisplayMode(NS_DisplayMode mode)
    {
        modes.push_back(mode);
        return modes.size()-1;
    }

    void Window::setDisplayMode(size_t modeID)
    {
        if (modeID >= modes.size()) {
            error("Invalid display mode", modeID);
            return;
        }
        SDL_SetWindowSize(engineData::window, modes[modeID].width, modes[modeID].height);
        engineData::displayRatio = (float)engineData::gameWidth/(float)modes[modeID].width;
        glViewport(0,0,modes[modeID].width, modes[modeID].height);
        if (modes[modeID].fullscreen) SDL_SetWindowFullscreen(engineData::window, windowFlags | SDL_WINDOW_FULLSCREEN);
        else SDL_SetWindowFullscreen(engineData::window, windowFlags);
        engineData::displaymode = modeID;
        // change vars ?
    }
    void Window::nextDisplaymode()
    {
        static int m = 0;
        m = (m+1)%modes.size();
        setDisplayMode(m);
    }

    void Window::InitDrawing(bool clear)
    {
        if (clear)
        {
            glm::vec3 c = activeCamera3D()->getClearColor();
            glClearColor(c.r, c.g, c.b, 1.f);
            if (engineData::gameflags & 4) glClearColor(0.5f, 0.5f, 0.5f, 1.f);
            glClearDepth(1.0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        for (auto& l : engineData::layers) l.begin();
    }

    

    void Window::EndDrawing(void (*func)(Window*))
    {
        baseShader->start();
        bool cam3dexists = activeCamera3D()!=nullptr;
        baseShader->SetCameraPosition(cam3dexists?activeCamera3D()->getPosition():glm::vec3(0,0,0));
        glActiveTexture(GL_TEXTURE0);
        toggleCulling(false);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        if (func != nullptr)
        {
            func(this);
            return;
        }
        for (auto& l : engineData::layers)
        {
            float mi=1000000.f, ma=1000000.f;
            glm::vec4 col = {0,0,0,0};
            l.end();
            if (l.is_static)
            {
                baseShader->SetProjectionMatrix(glm::mat4(1.f));
                baseShader->SetViewMatrix(cam3dexists?activeCamera3D()->getCamStatic():glm::mat4(1.f));
            }
            else
            {
                baseShader->SetProjectionMatrix(cam3dexists?activeCamera3D()->getProjection(l.is_static):glm::mat4(1.f));
                baseShader->SetViewMatrix(cam3dexists?activeCamera3D()->getView(l.is_static):glm::mat4(1.f));
            }
            if (engineData::cam3d!=nullptr && !l.is_static) col = engineData::cam3d->getFog(mi,ma);
            baseShader->SetFog(mi, ma, col);
            l.renderBatch();
        }
        toggleCulling(true);
        
        baseShader->stop();
        TextureManager::ResetTexture();
        glDisable(GL_BLEND);

        SDL_GL_SwapWindow(m_window);
    }

    void Window::BindAsRenderTarget()
    {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glViewport(0,0,windowWidth/engineData::displayRatio,windowHeight/engineData::displayRatio);
        setCamBoundaries(windowWidth,windowHeight);
    }

}
