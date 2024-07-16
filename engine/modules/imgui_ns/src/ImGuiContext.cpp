#include <imgui_ns.hpp>

#include <platform/Wnd/Wnd.h>

#include "vendor/imgui_impl_opengl3.h"
// TODO: Other ?

#if defined(NS_WND_BACKEND_SDL2)
#include "vendor/imgui_impl_sdl.h"
#include <platform/Wnd/SDL_GL/SDL_GL_.h>
#elif defined(NS_WND_BACKEND_X11)
#include "imgui_impl_x11.h"
#include <platform/Wnd/X_GL/X_GL_.h>
extern IMGUI_IMPL_API int ImGui_ImplX11_EventHandler(XEvent &event, XEvent *next_event);
#else
#error "Can't use ImGui with this wnd backend: not implemented"
#endif

#include <NSEngine.hpp>
#include <memory.h>

#include <EventHandler.hpp>
#include <SDL2/SDL_events.h>

namespace ns {

void NSMOD_INIT_imgui() { new ImGuiContext(*get_main_window()); }

void NSMOD_SHUTDOWN_imgui() { delete ImGuiContext::getInstance(); }

struct ImGuiEventHandler : public ns::IEventHandler {
  virtual ~ImGuiEventHandler() {}
  bool handle_platform_event(ns::platform::Event *e) override {
    bool ret = false;
    ImGuiIO &io = ImGui::GetIO();
#if defined(NS_WND_BACKEND_SDL2)
    switch (e->evt.type) {
    case SDL_KEYDOWN:
    case SDL_KEYUP:
      ret = io.WantCaptureKeyboard;
      break;
    case SDL_MOUSEBUTTONUP:
    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEWHEEL:
    case SDL_MOUSEMOTION:
      ret = io.WantCaptureMouse;
      break;
    }
    ImGui_ImplSDL2_ProcessEvent(&e->evt);
#elif defined(NS_WND_BACKEND_X11)
    switch (e->evt.type) {
      case ButtonPress:
      case ButtonRelease:
      case MotionNotify:
        ret = io.WantCaptureMouse;
        break;
      case KeyPress:
      case KeyRelease:
        ret = io.WantCaptureKeyboard;
        break;
    }
    ImGui_ImplX11_ProcessEvent(&e->evt);
#endif
    return ret;
  }
};

static ImGuiContext *INSTANCE = nullptr;
static ImGuiEventHandler evhandler;

ImGuiContext::ImGuiContext(platform::Wnd *pWnd) {
  if (INSTANCE) {
    return;
  }
  INSTANCE = this;
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui_ImplOpenGL3_Init("#version 130");
#if defined(NS_WND_BACKEND_SDL2)
  ImGui_ImplSDL2_InitForOpenGL(pWnd->window, pWnd->glContext);
#elif defined(NS_WND_BACKEND_X11)
  ImGui_ImplX11_Init(pWnd->display, &pWnd->win);
#endif
  ImGuiIO &io = ImGui::GetIO();
  io.IniFilename = "";
  ImGui::StyleColorsDark();
  platform::wnd_add_event_handler(pWnd, &evhandler, 20);
}

ImGuiContext::~ImGuiContext() {
  if (this != INSTANCE)
    return;
  INSTANCE = nullptr;
  ImGui_ImplOpenGL3_Shutdown();
#if defined(NS_WND_BACKEND_SDL2)
  ImGui_ImplSDL2_Shutdown();
#elif defined(NS_WND_BACKEND_X11)
  ImGui_ImplX11_Shutdown();
#endif
  ImGui::DestroyContext();
}

ImGuiContext *ImGuiContext::getInstance() { return INSTANCE; }

void ImGuiContext::new_frame() {
  ImGui_ImplOpenGL3_NewFrame();
#if defined(NS_WND_BACKEND_SDL2)
  ImGui_ImplSDL2_NewFrame();
#elif defined(NS_WND_BACKEND_X11)
  ImGui_ImplX11_NewFrame(get_main_window()->get_width(), get_main_window()->get_height(), get_main_window()->get_width(), get_main_window()->get_height());
#endif
  ImGui::NewFrame();
}

void ImGuiContext::render() {
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

} // namespace ns
