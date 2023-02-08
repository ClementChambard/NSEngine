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
        void stop() { m_running = false; }

        /**
         * Changes the maximum amount of frame per seconds
         */
        void setMaxFps(int fps) { m_fps.setMaxFps(fps); }

        /**
         * Instance of the game engine
         */
        static IEngine* instance;
    private:

        void on_create_engine();
        void on_update_engine();
        void on_render_engine();
        void on_destroy_engine();

        bool m_running = true;

        FpsLimiter m_fps;

        Window m_window;

        SDL_Event m_event;

            //uint32_t gameflags;
            //Camera2D* cam2d;
            //Camera3D* cam3d;

            //std::vector<SpriteBatch*> layers;
            //int targetLayer;

            //float displayRatio;

            //int displaymode;

            //int debugLayer;

            //std::vector<EventProcessor*> eventProcessors;
};

/**
 * Get the engine instance
 */
inline IEngine* getInstance() { return IEngine::instance; }

}

#endif // ENGINE_H_
