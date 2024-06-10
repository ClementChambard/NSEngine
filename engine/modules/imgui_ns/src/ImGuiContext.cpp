#include <imgui_ns.hpp>

#include <platform/Wnd/Wnd.h>

#include "vendor/imgui_impl_opengl3.h"
// TODO: Other ?

#if WND_BACKEND == WND_BACKEND_SDL
#define NSMOD_IMGUI_WND_SDL
#include "vendor/imgui_impl_sdl.h"
#else // NS_PLATFORM_WND_SDL
// TODO: Other ?
#error "Can't use ImGui with this wnd backend: not implemented"
#endif

#include <NSEngine.hpp>

#include <EventHandler.hpp>
#include <SDL2/SDL_events.h>

#if WND_BACKEND == WND_BACKEND_SDL
#include <platform/Wnd/SDL_GL/SDL_GL_.h>
#endif
#if WND_BACKEND == WND_BACKEND_X11
#include <platform/Wnd/X_GL/X_GL_.h>
#endif

namespace ns {

struct ImGuiEventHandler : public ns::IEventHandler {
  virtual ~ImGuiEventHandler() {}
  bool handlePlatformEvent(ns::platform::Event *e) override {
    bool ret = false;
    ImGuiIO &io = ImGui::GetIO();
#ifdef NSMOD_IMGUI_WND_SDL
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
#endif
    return ret;
  }
};

static ImGuiContext *INSTANCE = nullptr;
static ImGuiEventHandler evhandler;

ImGuiContext::ImGuiContext(platform::Wnd* pWnd) {
  if (INSTANCE) {
    return;
  }
  INSTANCE = this;
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui_ImplOpenGL3_Init("#version 130");
#ifdef NSMOD_IMGUI_WND_SDL
  ImGui_ImplSDL2_InitForOpenGL(pWnd->window, pWnd->glContext);
#endif
  ImGuiIO &io = ImGui::GetIO();
  io.IniFilename = "";
  ImGui::StyleColorsDark();
  platform::wnd_add_event_handler(pWnd, &evhandler, 20);
}

ImGuiContext::~ImGuiContext() {
  if (this != INSTANCE) return;
  INSTANCE = nullptr;
  ImGui_ImplOpenGL3_Shutdown();
#ifdef NSMOD_IMGUI_WND_SDL
  ImGui_ImplSDL2_Shutdown();
#endif
  ImGui::DestroyContext();
}

ImGuiContext* ImGuiContext::getInstance() {
  return INSTANCE;
}

void ImGuiContext::new_frame() {
  ImGui_ImplOpenGL3_NewFrame();
#ifdef NSMOD_IMGUI_WND_SDL
  ImGui_ImplSDL2_NewFrame();
#endif
  ImGui::NewFrame();
}

void ImGuiContext::render() {
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

}
