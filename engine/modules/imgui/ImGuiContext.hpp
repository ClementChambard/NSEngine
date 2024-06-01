#ifndef IMGUICONTEXT_INCLUDED_H
#define IMGUICONTEXT_INCLUDED_H

#include <imguiW.hpp>

namespace ns {

namespace platform { struct Wnd; }

class ImGuiContext {
public:
  ImGuiContext(platform::Wnd *pWnd);
  ~ImGuiContext();
  static ImGuiContext *getInstance();
};

} // namespace ns

#endif // IMGUICONTEXT_INCLUDED_H
