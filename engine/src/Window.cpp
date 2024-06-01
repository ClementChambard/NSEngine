#include "Window.h"
#include "NSEngine.hpp"
#include "logger.h"
#include "./TextureManager.h"
#include "./Camera3D.h"
#include "./SpriteBatch.h"
#include <glm/fwd.hpp>

namespace ns {

Window::Window() {}
Window::~Window() {}

void Window::Init(cstr name, u32 width, u32 height, u32 flag) {
  baseWindowWidth = windowWidth = width;
  baseWindowHeight = windowHeight = height;
  windowFlags = flag;

  platform::wnd_init(&m_platform, name, width, height, flag);

  addDisplayMode({width, height, (flag & NS_WND_FULLSCREEN) != 0});

  GLenum Error = glewInit();
  if (Error != GLEW_OK) {
    NS_FATAL("Failed to initialize GLEW: %s", glewGetErrorString(Error));
    exit(1);
  }

  baseShader = new AnimShader();

  NS_INFO("Window successfuly initialized");
}

void Window::destroy() {
  delete baseShader;
  baseShader = nullptr;
  platform::wnd_destroy(m_platform);
  m_platform = nullptr;
}

u32 Window::addDisplayMode(NS_DisplayMode mode) {
  modes.push_back(mode);
  return modes.size() - 1;
}

void Window::setDisplayMode(usize modeID) {
  if (modeID >= modes.size()) {
    NS_ERROR("Invalid display mode %llu", modeID);
    return;
  }
  platform::wnd_set_size(m_platform, modes[modeID].width, modes[modeID].height, modes[modeID].fullscreen);

  windowWidth = modes[modeID].width;
  windowHeight = modes[modeID].height;
  glViewport(0, 0, modes[modeID].width, modes[modeID].height);
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

void Window::swapBuffers() const {
  platform::wnd_swap_buffers(m_platform);
}

} // namespace NSEngine
