#include "Window.hpp"
#include "logger.h"

// TODO: Move gl specific stuff elsewhere
#include <GL/glew.h>

namespace ns {

Window::~Window() {}

void Window::configure(Window::Config const& cfg) {
  m_config = cfg;
}

void Window::open() {
  if (m_isOpen) {
    NS_WARN("Trying to open a window already open.");
    return;
  }
  platform::wnd_init(&m_platform, m_config.name, m_config.width, m_config.height, m_config.flags);
  platform::wnd_add_event_handler(m_platform, &m_handler, 0);
 
  m_width = m_config.width;
  m_height = m_config.height;
  m_isOpen = true;
}

void Window::close() {
  if (!m_isOpen) {
    NS_WARN("Trying to close a window that is not open.");
    return;
  }
  platform::wnd_destroy(m_platform);
  m_platform = nullptr;
  m_isOpen = false;
}

void Window::add_event_handler(IEventHandler *eh, u32 priority) {
  if (!m_isOpen) {
    NS_WARN("Event handler will not be added to closed window.   (TODO)");
    return;
  }
  platform::wnd_add_event_handler(m_platform, eh, priority);
}

void Window::process_events() {
  if (!m_isOpen) {
    NS_WARN("Not processing event of closed window.");
  }
  platform::wnd_handle_events(m_platform);
  if (m_shouldClose) {
    close();
    m_shouldClose = false;
  }
}

void Window::bind_as_render_target() {
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
  glViewport(0, 0, m_width, m_height);
}

void Window::swap_buffers() const {
  platform::wnd_swap(m_platform);
}

bool Window::WindowHandler::on_quit() {
  w->m_shouldClose = true;
  return false;
}

bool Window::WindowHandler::on_resize(u32 w, u32 h) {
  this->w->m_width = w;
  this->w->m_height = h;
  return false;
}

} // namespace ns
