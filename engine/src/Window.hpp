#ifndef WINDOW
#define WINDOW

#include "./defines.h"
#include "platform/Wnd/Wnd.h"
#include "EventHandler.hpp"

namespace ns {

/**
 * NSEngine's window class
 */
class Window {
public:
    Window() {}
    ~Window();

    struct Config {
        cstr name;
        u32 width;
        u32 height;
        u32 flags;
    };

    /**
     * Configure the window before opening it
     */
    void configure(Config const& cfg);

    /**
     * Open the window
     */
    void open();

    /**
     * Close the window
     */
    void close();

    /**
     * Adds an handler to capture window events. lowest priority value runs first.
     */
    void addEventHandler(IEventHandler* eh, u32 priority = 1);

    /**
     * Dispatches window events to the attached handlers
     */
    void processEvents();

    /**
     * Sets the draw target to this window
     */
    void BindAsRenderTarget();

    Config const& getConfig() const { return m_config; }
    u32 getWidth() const { return m_width; }
    u32 getHeight() const { return m_height; }
    bool isOpen() const { return m_isOpen; }

    operator platform::Wnd*() const { return m_platform; }

    void swapBuffers() const;

private:
    Config m_config{};
    u32 m_width = 0;
    u32 m_height = 0;
    u32 m_isOpen:1 = false;
    u32 m_shouldClose:1 = false;
    u32 m_reservedFlags:30 = 0;
    platform::Wnd* m_platform = nullptr;
    
    struct WindowHandler : public IEventHandler {
        Window* w;
        WindowHandler(Window* w) : IEventHandler(), w(w) {}
        virtual ~WindowHandler() {}
        bool handleQuit() override;
        bool handleResize(u32, u32) override;
    } m_handler{this};
};

}

#endif
