#ifndef WINDOW
#define WINDOW

#include "AnimShader.h"
#include "vertex.h"
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <vector>

namespace NSEngine {

    /**
     * Contains the window width and heighr in pixels and if the window is fullscreen.
     */
    struct NS_DisplayMode {
        int width, height;
        bool fullscreen;
    };


    /**
     * NSEngine's window class
     */
    class Window {
        public:
            Window();
            ~Window();

            /**
             * Initialize the window object.
             */
            void Init(const char* name, int width, int height, int flag);

            /**
             * Destroys the window object
             */
            void destroy();

            /**
             * Adds a new display mode to the window object
             */
            int addDisplayMode(NS_DisplayMode mode);

            /**
             * Changes the display mode of the window object
             */
            void setDisplayMode(int modeID);

            /**
             * Switches to the next display mode
             */
            void nextDisplaymode();

            /**
             * Call this function when you want to start the drawing on the window object
             */
            void InitDrawing(bool clear = true);

            /**
             * Call this function when you want to stop the drawing on the window object
             * You can also use a custom function
             */
            void EndDrawing(void (*func)(Window*) = nullptr);

            /**
             * Sets the draw target to this window
             */
            void BindAsRenderTarget();

            int windowFlags, windowWidth, windowHeight;
            AnimShader* baseShader;
     
        private:
            SDL_GLContext context;
            std::vector<NS_DisplayMode> modes;
    };


}

#endif
