#ifndef ENGINE_H_
#define ENGINE_H_

#include "Camera3D.h"
#include "EventProcessor.h"
#include "SpriteBatch.h"
#include "Timing.h"
#include "Window.h"
#include "./defines.h"
#include "./logger.h"

namespace ns {

class IEngine {
    public:
        IEngine(i32 width, i32 height, cstr name);
        virtual ~IEngine();
        IEngine(IEngine const&) = delete;
        IEngine(IEngine&&) = delete;
        IEngine& operator=(IEngine const&) = delete;
        IEngine& operator=(IEngine&&) = delete;

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

        virtual void on_render_pre() {}
        virtual void on_render_post() {}
        /**
         * user defined destroy event
         */
        virtual void on_destroy() { NS_WARN("default on_destroy called"); }

        /**
         * Run the app
         */
        void run();

        /**
         * Stops the engine
         */
        void stop() { m_gameflags.flags.running = false; }

        /**
         * Changes the maximum amount of frame per seconds
         */
        void setMaxFps(i32 fps) { m_fps.setMaxFps(fps); }

        /**
         * Instance of the game engine
         */
        static IEngine* instance;

        /**
         * Getter for the game flags (as reference so possible to change it here)
         */
        auto& flags() { return m_gameflags; }

        /**
         * Getter for the game window
         */
        Window& window() { return m_window; }

        void setRenderFunc(void (*render_func)(Window*)) { this->render_func = render_func; }

        f32 fps() const { return m_fps.GetFPS(); }
        f32 gameSpeed() const { return m_gameSpeed; }

        void setGameSpeed(f32 gs) { m_gameSpeed = gs; }

        void createCamera(i32 width, i32 height) { m_cam3d = new Camera3D(width, height); }

        void addEventProcessor(IEventProcessor* ip) { m_eventProcessors.push_back(ip); }

        i32 addGameLayer(bool depthTest = false, bool is_static = false);
    private:

        void on_create_engine();
        void on_update_engine();
        void on_render_engine();
        void on_destroy_engine();

        FpsLimiter m_fps;

        Window m_window;

        f32 m_gameSpeed = 1.0f;

        void (*render_func)(Window*) = nullptr;

        union {
            u32 val;
            struct {
                u32 running:1;
                u32 debugInfo:1;
                u32 wireframe:1;
                u32 framebyframe:1;
                u32 freecam:1;
            } flags;
        } m_gameflags;

        Camera3D* m_cam3d = nullptr;

        std::vector<IEventProcessor*> m_eventProcessors;

        std::vector<SpriteBatch> m_layers;

        friend Camera3D* activeCamera3D();
        friend std::vector<SpriteBatch>& getGameLayers();
};

/**
 * Get the engine instance
 */
inline IEngine* getInstance() { return IEngine::instance; }

/**
* Gets the current active camera3d
*/
Camera3D* activeCamera3D();

/**
* Gets the layer list
*/
std::vector<SpriteBatch>& getGameLayers();

extern i32 DEBUG_LAYER_ID;

}

#endif // ENGINE_H_
