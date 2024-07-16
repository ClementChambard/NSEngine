#ifndef IMGUINS_HEADER_INCLUDED
#define IMGUINS_HEADER_INCLUDED

#include <math/types/types.hpp>
#include <imgui.h>
#include <imgui_internal.h>

namespace ns {

void NSMOD_INIT_imgui();
void NSMOD_SHUTDOWN_imgui();

namespace platform { struct Wnd; }

class ImGuiContext {
public:
  ImGuiContext(platform::Wnd *pWnd);
  ~ImGuiContext();
  static ImGuiContext *getInstance();
  void new_frame();
  void render();
};

} // namespace ns

#define IM_VEC2_CLASS_EXTRA \
    ImVec2(ns::vec2 const& v) : x(v.x), y(v.y) {} \
    operator ns::vec2() { return {x, y}; }

#define IM_VEC4_CLASS_EXTRA \
    ImVec4(ns::vec4 const& v) : x(v.x), y(v.y), z(v.z), w(v.w) {} \
    operator ns::vec4() { return {x, y, z, w}; }


inline ImVec2 operator+(ImVec2 const& v1, ImVec2 const& v2) {
    return ImVec2(v1.x + v2.x, v1.y + v2.y);
}

inline ImVec2 operator-(ImVec2 const& v1) {
    return ImVec2(-v1.x, -v1.y);
}

inline ImVec2 operator-(ImVec2 const& v1, ImVec2 const& v2) {
    return v1 + -v2;
}

inline ImVec2 operator*(ImVec2 const& v1, ImVec2 const& v2) {
    return ImVec2(v1.x * v2.x, v1.y * v2.y);
}

inline ImVec2 operator*(ImVec2 const& v1, float const f) {
    return ImVec2(v1.x * f, v1.y * f);
}

inline ImVec2 operator*(float const f, ImVec2 const& v1) {
    return ImVec2(v1.x * f, v1.y * f);
}

inline ImVec2 operator/(ImVec2 const& v1, ImVec2 const& v2) {
    return ImVec2(v1.x / v2.x, v1.y / v2.y);
}

inline ImVec2 operator/(ImVec2 const& v1, float const f) {
    return ImVec2(v1.x / f, v1.y / f);
}

#endif // IMGUINS_HEADER_INCLUDED
