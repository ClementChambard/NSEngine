#ifndef ENGINE_H_
#define ENGINE_H_

#include "Timing.h"
#include "Window.h"
#include "Error.h"

namespace NSEngine {

class IEngine {
    public:
        IEngine(int width, int height, std::string name);
        virtual ~IEngine();
        IEngine(IEngine const&) = delete;
        IEngine(IEngine&&) = delete;
        IEngine& operator=(IEngine const&) = delete;
        IEngine& operator=(IEngine&&) = delete;

        /**
         * user defined create event
         */
        virtual void on_create() { warning("default on_create called"); }

        /**
         * user defined update event
         */
        virtual void on_update() { warning("default on_update called"); }

        /**
         * user defined render event
         */
        virtual void on_render() { warning("default on_render called"); }

        virtual void on_render_pre() {}
        virtual void on_render_post() {}
        /**
         * user defined destroy event
         */
        virtual void on_destroy() { warning("default on_destroy called"); }

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
        void setMaxFps(int fps) { m_fps.setMaxFps(fps); }

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

        float fps() const { return m_fps.GetFPS(); }
    private:

        void on_create_engine();
        void on_update_engine();
        void on_render_engine();
        void on_destroy_engine();

        FpsLimiter m_fps;

        Window m_window;

        SDL_Event m_event;

        void (*render_func)(Window*) = nullptr;

        union {
            uint32_t val;
            struct {
                uint32_t running:1;
                uint32_t debugInfo:1;
                uint32_t wireframe:1;
                uint32_t framebyframe:1;
                uint32_t freecam:1;
            } flags;
        } m_gameflags;

            //Camera2D* cam2d;
            //Camera3D* cam3d;

            //std::vector<SpriteBatch*> layers;
            //int targetLayer;

            //int debugLayer;

            //std::vector<EventProcessor*> eventProcessors;
};

/**
 * Get the engine instance
 */
inline IEngine* getInstance() { return IEngine::instance; }

}

#endif // ENGINE_H_
