#include "./ImGuiContext.hpp"

#include <SDL2/SDL.h>
#include <InputManager.h>
#include <NSEngine.hpp>

namespace ns {

namespace platform {

struct Wnd {
  SDL_Window *window;
  SDL_GLContext glContext;
};

}

class ImGuiEventProcessor : public ns::IEventProcessor {
public:
  ~ImGuiEventProcessor() override {}
  void ProcessEvent(SDL_Event *e, bool &noKeyboard, bool &noMouse) override {
    ImGui_ImplSDL2_ProcessEvent(e);

    ImGuiIO &io = ImGui::GetIO();
    noKeyboard = io.WantCaptureKeyboard;
    noMouse = io.WantCaptureMouse;
  }
};

static ImGuiEventProcessor *igep = nullptr;
static ImGuiContext *INSTANCE = nullptr;

ImGuiContext::ImGuiContext(platform::Wnd* pWnd) {
  if (INSTANCE) {
    return;
  }
  INSTANCE = this;
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui_ImplOpenGL3_Init("#version 130");
  ImGui_ImplSDL2_InitForOpenGL(pWnd->window, pWnd->glContext);
  ImGuiIO &io = ImGui::GetIO();
  io.IniFilename = "";
  ImGui::StyleColorsDark();
  igep = new ImGuiEventProcessor();
  ns::getInstance()->addEventProcessor(igep);
  ns::InputManager::SetAsEventProcessor();
}

ImGuiContext::~ImGuiContext() {
  if (this != INSTANCE) return;
  INSTANCE = nullptr;
  delete igep;
  igep = nullptr;
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();
}

ImGuiContext* ImGuiContext::getInstance() {
  return INSTANCE;
}

}
