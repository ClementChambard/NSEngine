#ifndef WINDOW
#define WINDOW

#include "AnimShader.h"
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <vector>
#include "./defines.h"

namespace ns {

/**
 * Contains the window width and heighr in pixels and if the window is fullscreen.
 */
struct NS_DisplayMode {
    i32 width, height;
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
        void Init(cstr name, i32 width, i32 height, i32 flag);

        /**
         * Destroys the window object
         */
        void destroy();

        /**
         * Adds a new display mode to the window object
         */
        i32 addDisplayMode(NS_DisplayMode mode);

        /**
         * Changes the display mode of the window object
         */
        void setDisplayMode(usize modeID);

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

        auto getWindowData() const { struct {i32 flags; i32 width; i32 height; i32 bwidth; i32 bheight;} r = {windowFlags, windowWidth, windowHeight, baseWindowWidth, baseWindowHeight}; return r; }

        AnimShader* getBaseShader() const { return baseShader; }
        SDL_Window* getSdlWindow() const { return m_window; }
        SDL_GLContext getSdlGlContext() const { return m_context; }
        i32 getDisplayMode() const { return displayMode; }

    private:
        i32 displayMode = 0;
        std::vector<NS_DisplayMode> modes;
        i32 windowFlags = 0, baseWindowWidth = 0, baseWindowHeight = 0;
        i32 windowWidth = 0, windowHeight = 0;
        AnimShader* baseShader = nullptr;
        SDL_Window* m_window = nullptr;
        SDL_GLContext m_context = nullptr;
};

}

#endif
