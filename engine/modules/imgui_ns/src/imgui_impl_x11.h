#pragma once
#include "imgui.h"      // IMGUI_IMPL_API

typedef unsigned long Window;
typedef struct _XDisplay Display;
typedef struct __GLXcontextRec *GLXContext;
typedef union _XEvent XEvent;

IMGUI_IMPL_API bool     ImGui_ImplX11_Init(Display* display, Window* window);
IMGUI_IMPL_API void     ImGui_ImplX11_Shutdown();
IMGUI_IMPL_API void     ImGui_ImplX11_NewFrame(int w, int h, int display_w, int display_h);
IMGUI_IMPL_API bool     ImGui_ImplX11_ProcessEvent(const XEvent* event);
