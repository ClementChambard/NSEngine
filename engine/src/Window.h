#ifndef WINDOW
#define WINDOW

#include "AnimShader.h"
#include "./platform/platform_wnd.h"
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <vector>
#include "./defines.h"

namespace ns {

/**
 * Contains the window width and heighr in pixels and if the window is fullscreen.
 */
struct NS_DisplayMode {
    u32 width, height;
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
        void Init(cstr name, u32 width, u32 height, u32 flag);

        /**
         * Destroys the window object
         */
        void destroy();

        /**
         * Adds a new display mode to the window object
         */
        u32 addDisplayMode(NS_DisplayMode mode);

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

        auto getWindowData() const { struct {u32 flags; u32 width; u32 height; u32 bwidth; u32 bheight;} r = {windowFlags, windowWidth, windowHeight, baseWindowWidth, baseWindowHeight}; return r; }

        platform::Wnd* getPlatformWindow() const { return m_platform; }
        u32 getDisplayMode() const { return displayMode; }
        void swapBuffers() const;

    private:
        u32 displayMode = 0;
        std::vector<NS_DisplayMode> modes;
        u32 windowFlags = 0, baseWindowWidth = 0, baseWindowHeight = 0;
        u32 windowWidth = 0, windowHeight = 0;
        AnimShader* baseShader = nullptr;
        platform::Wnd* m_platform = nullptr;
};

}

#endif
