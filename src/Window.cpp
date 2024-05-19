#include "Window.h"
#include "NSEngine.hpp"
#include "Error.h"
#include "./TextureManager.h"
#include "./Camera3D.h"
#include "./SpriteBatch.h"
#include <glm/fwd.hpp>

namespace ns {

Window::Window() {}
Window::~Window() {}

void Window::Init(cstr name, i32 width, i32 height, i32 flag) {
  baseWindowWidth = windowWidth = width;
  baseWindowHeight = windowHeight = height;
  windowFlags = flag | SDL_WINDOW_OPENGL;

  addDisplayMode({width, height, (flag & SDL_WINDOW_FULLSCREEN) != 0});

  m_window =
      SDL_CreateWindow(name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                       width, height, windowFlags);
  if (m_window == nullptr)
    fatalError("Failed to create window");

  m_context = SDL_GL_CreateContext(m_window);
  if (m_context == nullptr)
    fatalError("Failed to create GL context");

  GLenum Error = glewInit();
  if (Error != GLEW_OK)
    fatalError("Failed to initialize GLEW:", glewGetErrorString(Error));

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetSwapInterval(1);

  glClearColor(0.f, 0.f, 0.f, 1.f);
  glEnable(GL_MULTISAMPLE);

  baseShader = new AnimShader();

  info("Window successfuly initialized");
}

void Window::destroy() {
  delete baseShader;
  baseShader = nullptr;
  SDL_GL_DeleteContext(m_context);
  SDL_DestroyWindow(m_window);
}

i32 Window::addDisplayMode(NS_DisplayMode mode) {
  modes.push_back(mode);
  return modes.size() - 1;
}

void Window::setDisplayMode(usize modeID) {
  if (modeID >= modes.size()) {
    error("Invalid display mode", modeID);
    return;
  }
  SDL_SetWindowSize(m_window, modes[modeID].width, modes[modeID].height);
  windowWidth = modes[modeID].width;
  windowHeight = modes[modeID].height;
  glViewport(0, 0, modes[modeID].width, modes[modeID].height);
  if (modes[modeID].fullscreen)
    SDL_SetWindowFullscreen(m_window, windowFlags | SDL_WINDOW_FULLSCREEN);
  else
    SDL_SetWindowFullscreen(m_window, windowFlags);
  displayMode = modeID;
}
void Window::nextDisplaymode() {
  i32 m = (displayMode + 1) % modes.size();
  setDisplayMode(m);
}

void Window::InitDrawing(bool clear) {
  if (clear) {
    glm::vec3 c = activeCamera3D() ? activeCamera3D()->getClearColor() : glm::vec3{0.f,0.f,0.f};
    glClearColor(c.r, c.g, c.b, 1.f);
    if (getInstance()->flags().flags.wireframe)
      glClearColor(0.5f, 0.5f, 0.5f, 1.f);
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

  for (auto &l : getGameLayers())
    l.begin();
}

void Window::EndDrawing(void (*func)(Window *)) {
  baseShader->start();
  bool cam3dexists = activeCamera3D() != nullptr;
  baseShader->SetCameraPosition(cam3dexists ? activeCamera3D()->getPosition()
                                            : glm::vec3(0, 0, 0));
  glActiveTexture(GL_TEXTURE0);
  glDisable(GL_CULL_FACE);

  if (getInstance()->flags().flags.wireframe) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  } else {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  }

  if (func != nullptr) {
    func(this);
    SDL_GL_SwapWindow(m_window);
    return;
  }

  for (size_t i = 0; i < getGameLayers().size(); i++) {
    auto& l = getGameLayers()[i];
    f32 mi = 1000000.f, ma = 1000000.f;
    glm::vec4 col = {0, 0, 0, 0};
    l.end();
    if (l.is_static) {
      baseShader->SetProjectionMatrix(glm::mat4(1.f));
      baseShader->SetViewMatrix(cam3dexists ? activeCamera3D()->getCamStatic()
                                            : glm::mat4(1.f));
    } else {
      baseShader->SetProjectionMatrix(
          cam3dexists ? activeCamera3D()->getProjection(l.is_static)
                      : glm::mat4(1.f));
      baseShader->SetViewMatrix(cam3dexists
                                    ? activeCamera3D()->getView(l.is_static)
                                    : glm::mat4(1.f));
    }
    if (activeCamera3D() != nullptr && !l.is_static)
      col = activeCamera3D()->getFog(mi, ma);
    baseShader->SetFog(mi, ma, col);
    l.renderBatch();
  }
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  baseShader->stop();
  TextureManager::ResetTexture();
  glDisable(GL_BLEND);

  if (getInstance()->flags().flags.wireframe) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }
}

void Window::BindAsRenderTarget() {
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
  glViewport(0, 0, windowWidth, windowHeight);
  if (activeCamera3D() != nullptr) activeCamera3D()->setWH(baseWindowWidth, baseWindowHeight, 1.f);
}

} // namespace NSEngine
