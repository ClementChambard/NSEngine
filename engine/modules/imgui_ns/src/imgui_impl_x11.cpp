#include "imgui.h"
#include "imgui_impl_x11.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <GL/gl.h>
#include <GL/glx.h>

#include <chrono>

struct ImGui_ImplX11_Data
{
    Display* display;
    Window* window;
    std::chrono::time_point<std::chrono::high_resolution_clock> Time;
    int             MouseButtonsDown;
    // TODO:
    // SDL_Cursor*     MouseCursors[ImGuiMouseCursor_COUNT];
    int             PendingMouseLeaveFrame;
    char*           ClipboardTextData;
    bool            MouseCanUseGlobalState;

    ImGui_ImplX11_Data()   { memset((void*)this, 0, sizeof(*this)); }
};

// Backend data stored in io.BackendPlatformUserData to allow support for multiple Dear ImGui contexts
// It is STRONGLY preferred that you use docking branch with multi-viewports (== single Dear ImGui context + multiple windows) instead of multiple Dear ImGui contexts.

static ImGui_ImplX11_Data* ImGui_ImplX11_GetBackendData()
{
    return ImGui::GetCurrentContext() ? (ImGui_ImplX11_Data*)ImGui::GetIO().BackendPlatformUserData : NULL;
}

// Functions
static const char* ImGui_ImplX11_GetClipboardText(void*)
{
    // ImGui_ImplX11_Data* bd = ImGui_ImplX11_GetBackendData();
    // if (bd->ClipboardTextData)
    //     SDL_free(bd->ClipboardTextData);
    // bd->ClipboardTextData = SDL_GetClipboardText();
    // return bd->ClipboardTextData;
    return nullptr;
}

static void ImGui_ImplX11_SetClipboardText(void*, const char* /*text*/)
{
    // SDL_SetClipboardText(text);
}

static ImGuiKey ImGui_ImplX11_KeycodeToImGuiKey(int keycode)
{
    switch (keycode)
    {
        case XK_Tab: return ImGuiKey_Tab;
        case XK_Left: return ImGuiKey_LeftArrow;
        case XK_Right: return ImGuiKey_RightArrow;
        case XK_Up: return ImGuiKey_UpArrow;
        case XK_Down: return ImGuiKey_DownArrow;
        case XK_Page_Up: return ImGuiKey_PageUp;
        case XK_Page_Down: return ImGuiKey_PageDown;
        case XK_Home: return ImGuiKey_Home;
        case XK_End: return ImGuiKey_End;
        case XK_Insert: return ImGuiKey_Insert;
        case XK_Delete: return ImGuiKey_Delete;
        case XK_BackSpace: return ImGuiKey_Backspace;
        case XK_space: return ImGuiKey_Space;
        case XK_Return: return ImGuiKey_Enter;
        case XK_Escape: return ImGuiKey_Escape;
        case XK_apostrophe: return ImGuiKey_Apostrophe;
        case XK_comma: return ImGuiKey_Comma;
        case XK_minus: return ImGuiKey_Minus;
        case XK_period: return ImGuiKey_Period;
        case XK_slash: return ImGuiKey_Slash;
        case XK_semicolon: return ImGuiKey_Semicolon;
        case XK_equal: return ImGuiKey_Equal;
        case XK_bracketleft: return ImGuiKey_LeftBracket;
        case XK_backslash: return ImGuiKey_Backslash;
        case XK_braceright: return ImGuiKey_RightBracket;
        case XK_grave: return ImGuiKey_GraveAccent;
        case XK_Caps_Lock: return ImGuiKey_CapsLock;
        case XK_Scroll_Lock: return ImGuiKey_ScrollLock;
        case XK_Num_Lock: return ImGuiKey_NumLock;
        case XK_Print: return ImGuiKey_PrintScreen;
        case XK_Pause: return ImGuiKey_Pause;
        case XK_KP_0: return ImGuiKey_Keypad0;
        case XK_KP_1: return ImGuiKey_Keypad1;
        case XK_KP_2: return ImGuiKey_Keypad2;
        case XK_KP_3: return ImGuiKey_Keypad3;
        case XK_KP_4: return ImGuiKey_Keypad4;
        case XK_KP_5: return ImGuiKey_Keypad5;
        case XK_KP_6: return ImGuiKey_Keypad6;
        case XK_KP_7: return ImGuiKey_Keypad7;
        case XK_KP_8: return ImGuiKey_Keypad8;
        case XK_KP_9: return ImGuiKey_Keypad9;
        case XK_KP_Decimal: return ImGuiKey_KeypadDecimal;
        case XK_KP_Divide: return ImGuiKey_KeypadDivide;
        case XK_KP_Multiply: return ImGuiKey_KeypadMultiply;
        case XK_KP_Subtract: return ImGuiKey_KeypadSubtract;
        case XK_KP_Add: return ImGuiKey_KeypadAdd;
        case XK_KP_Enter: return ImGuiKey_KeypadEnter;
        case XK_KP_Equal: return ImGuiKey_KeypadEqual;
        case XK_Control_L: return ImGuiKey_LeftCtrl;
        case XK_Shift_L: return ImGuiKey_LeftShift;
        case XK_Alt_L: return ImGuiKey_LeftAlt;
        case XK_Super_L: return ImGuiKey_LeftSuper;
        case XK_Control_R: return ImGuiKey_RightCtrl;
        case XK_Shift_R: return ImGuiKey_RightShift;
        case XK_Alt_R: return ImGuiKey_RightAlt;
        case XK_Super_R: return ImGuiKey_RightSuper;
        case XK_Menu: return ImGuiKey_Menu;
        case XK_0: return ImGuiKey_0;
        case XK_1: return ImGuiKey_1;
        case XK_2: return ImGuiKey_2;
        case XK_3: return ImGuiKey_3;
        case XK_4: return ImGuiKey_4;
        case XK_5: return ImGuiKey_5;
        case XK_6: return ImGuiKey_6;
        case XK_7: return ImGuiKey_7;
        case XK_8: return ImGuiKey_8;
        case XK_9: return ImGuiKey_9;
        case XK_a: 
        case XK_A: return ImGuiKey_A;
        case XK_b:
        case XK_B: return ImGuiKey_B;
        case XK_c:
        case XK_C: return ImGuiKey_C;
        case XK_d:
        case XK_D: return ImGuiKey_D;
        case XK_e:
        case XK_E: return ImGuiKey_E;
        case XK_f:
        case XK_F: return ImGuiKey_F;
        case XK_g:
        case XK_G: return ImGuiKey_G;
        case XK_h:
        case XK_H: return ImGuiKey_H;
        case XK_i:
        case XK_I: return ImGuiKey_I;
        case XK_j:
        case XK_J: return ImGuiKey_J;
        case XK_k:
        case XK_K: return ImGuiKey_K;
        case XK_l:
        case XK_L: return ImGuiKey_L;
        case XK_m:
        case XK_M: return ImGuiKey_M;
        case XK_n:
        case XK_N: return ImGuiKey_N;
        case XK_o:
        case XK_O: return ImGuiKey_O;
        case XK_p:
        case XK_P: return ImGuiKey_P;
        case XK_q:
        case XK_Q: return ImGuiKey_Q;
        case XK_r:
        case XK_R: return ImGuiKey_R;
        case XK_s:
        case XK_S: return ImGuiKey_S;
        case XK_t:
        case XK_T: return ImGuiKey_T;
        case XK_u:
        case XK_U: return ImGuiKey_U;
        case XK_v:
        case XK_V: return ImGuiKey_V;
        case XK_w:
        case XK_W: return ImGuiKey_W;
        case XK_x:
        case XK_X: return ImGuiKey_X;
        case XK_y:
        case XK_Y: return ImGuiKey_Y;
        case XK_z:
        case XK_Z: return ImGuiKey_Z;
        case XK_F1: return ImGuiKey_F1;
        case XK_F2: return ImGuiKey_F2;
        case XK_F3: return ImGuiKey_F3;
        case XK_F4: return ImGuiKey_F4;
        case XK_F5: return ImGuiKey_F5;
        case XK_F6: return ImGuiKey_F6;
        case XK_F7: return ImGuiKey_F7;
        case XK_F8: return ImGuiKey_F8;
        case XK_F9: return ImGuiKey_F9;
        case XK_F10: return ImGuiKey_F10;
        case XK_F11: return ImGuiKey_F11;
        case XK_F12: return ImGuiKey_F12;
    }
    return ImGuiKey_None;
}

// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
// If you have multiple SDL events and some of them are not meant to be used by dear imgui, you may need to filter events based on their windowID field.
bool ImGui_ImplX11_ProcessEvent(const XEvent* event)
{
    ImGuiIO& io = ImGui::GetIO();
    ImGui_ImplX11_Data* bd = ImGui_ImplX11_GetBackendData();

    switch (event->type)
    {
    case MotionNotify:
    {
        io.AddMousePosEvent((float)event->xmotion.x, (float)event->xmotion.y);
        return true;
    }
    case ButtonPress:
    case ButtonRelease:
    {
        if (event->xbutton.button == 4) { io.AddMouseWheelEvent(1.0f, 0.0f); break; }
        if (event->xbutton.button == 5) { io.AddMouseWheelEvent(-1.0f, 0.0f); break; }
        if (event->xbutton.button == 6) { io.AddMouseWheelEvent(0.0f, 1.0f); break; }
        if (event->xbutton.button == 7) { io.AddMouseWheelEvent(0.0f, -1.0f); break; }
        int mouse_button = -1;
        if (event->xbutton.button == 1) { mouse_button = 0; }
        if (event->xbutton.button == 3) { mouse_button = 1; }
        if (event->xbutton.button == 2) { mouse_button = 2; }
        if (event->xbutton.button == 8) { mouse_button = 3; }
        if (event->xbutton.button == 9) { mouse_button = 4; }
        if (mouse_button == -1)
            break;
        io.AddMouseButtonEvent(mouse_button, (event->type == ButtonPress));
        bd->MouseButtonsDown = (event->type == ButtonPress) ? (bd->MouseButtonsDown | (1 << mouse_button)) : (bd->MouseButtonsDown & ~(1 << mouse_button));
        return true;
    }
    // case SDL_TEXTINPUT:
    // {
    //     io.AddInputCharactersUTF8(event->text.text);
    //     return true;
    // }
    case KeyPress:
    case KeyRelease:
    {
        // ImGui_ImplX11_UpdateKeyModifiers((SDL_Keymod)event->key.keysym.mod);
        XKeyEvent * evt = const_cast<XKeyEvent*>(&event->xkey);
        KeySym key_sym = XLookupKeysym(evt, 0);
        ImGuiKey key = ImGui_ImplX11_KeycodeToImGuiKey(key_sym);

        io.AddKeyEvent(key, (event->type == KeyPress));
        io.SetKeyEventNativeData(key, event->xkey.keycode, event->xkey.keycode, event->xkey.keycode);
        // To support legacy indexing (<1.87 user code). Legacy backend uses SDLK_*** as indices to IsKeyXXX() functions.
        return true;
    }
    case EnterNotify:
      bd->PendingMouseLeaveFrame = 0;
      return true;
    case LeaveNotify:
      bd->PendingMouseLeaveFrame = ImGui::GetFrameCount() + 1;
      return true;
    case FocusIn:
      io.AddFocusEvent(true);
      return true;
    case FocusOut:
      io.AddFocusEvent(false);
      return true;
    }
    return false;
}

bool ImGui_ImplX11_Init(Display* display, Window* window)
{
    ImGuiIO& io = ImGui::GetIO();
    IM_ASSERT(io.BackendPlatformUserData == NULL && "Already initialized a platform backend!");

    // Check and store if we are on a SDL backend that supports global mouse position
    // ("wayland" and "rpi" don't support it, but we chose to use a white-list instead of a black-list)
    bool mouse_can_use_global_state = false;
#if SDL_HAS_CAPTURE_AND_GLOBAL_MOUSE
    const char* sdl_backend = SDL_GetCurrentVideoDriver();
    const char* global_mouse_whitelist[] = { "windows", "cocoa", "x11", "DIVE", "VMAN" };
    for (int n = 0; n < IM_ARRAYSIZE(global_mouse_whitelist); n++)
        if (strncmp(sdl_backend, global_mouse_whitelist[n], strlen(global_mouse_whitelist[n])) == 0)
            mouse_can_use_global_state = true;
#endif

    // Setup backend capabilities flags
    ImGui_ImplX11_Data* bd = IM_NEW(ImGui_ImplX11_Data)();
    io.BackendPlatformUserData = (void*)bd;
    io.BackendPlatformName = "imgui_impl_sdl";
    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;       // We can honor GetMouseCursor() values (optional)
    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;        // We can honor io.WantSetMousePos requests (optional, rarely used)

    bd->window = window;
    bd->display = display;
    bd->MouseCanUseGlobalState = mouse_can_use_global_state;
    bd->Time = std::chrono::high_resolution_clock::now();

    io.SetClipboardTextFn = ImGui_ImplX11_SetClipboardText;
    io.GetClipboardTextFn = ImGui_ImplX11_GetClipboardText;
    io.ClipboardUserData = NULL;

    // TODO: Load mouse cursors
    // bd->MouseCursors[ImGuiMouseCursor_Arrow] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
    // bd->MouseCursors[ImGuiMouseCursor_TextInput] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM);
    // bd->MouseCursors[ImGuiMouseCursor_ResizeAll] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEALL);
    // bd->MouseCursors[ImGuiMouseCursor_ResizeNS] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENS);
    // bd->MouseCursors[ImGuiMouseCursor_ResizeEW] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEWE);
    // bd->MouseCursors[ImGuiMouseCursor_ResizeNESW] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENESW);
    // bd->MouseCursors[ImGuiMouseCursor_ResizeNWSE] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENWSE);
    // bd->MouseCursors[ImGuiMouseCursor_Hand] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
    // bd->MouseCursors[ImGuiMouseCursor_NotAllowed] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_NO);

    return true;
}

void ImGui_ImplX11_Shutdown()
{
    ImGui_ImplX11_Data* bd = ImGui_ImplX11_GetBackendData();
    IM_ASSERT(bd != NULL && "No platform backend to shutdown, or already shutdown?");
    ImGuiIO& io = ImGui::GetIO();

    // TODO: clipboard and cursors

    // if (bd->ClipboardTextData)
    //     SDL_free(bd->ClipboardTextData);
    // for (ImGuiMouseCursor cursor_n = 0; cursor_n < ImGuiMouseCursor_COUNT; cursor_n++)
    //     SDL_FreeCursor(bd->MouseCursors[cursor_n]);

    io.BackendPlatformName = NULL;
    io.BackendPlatformUserData = NULL;
    IM_DELETE(bd);
}

static void ImGui_ImplX11_UpdateMouseData()
{
    ImGui_ImplX11_Data* bd = ImGui_ImplX11_GetBackendData();
    ImGuiIO& io = ImGui::GetIO();

    // We forward mouse input when hovered or captured (via SDL_MOUSEMOTION) or when focused (below)
    // SDL_CaptureMouse() let the OS know e.g. that our imgui drag outside the SDL window boundaries shouldn't e.g. trigger other operations outside
  
    // TODO:

    // SDL_CaptureMouse(bd->MouseButtonsDown != 0 ? SDL_TRUE : SDL_FALSE);
    // SDL_Window* focused_window = SDL_GetKeyboardFocus();
    // const bool is_app_focused = (bd->Window == focused_window);

    if (true) // is_app_focused)
    {
        // (Optional) Set OS mouse position from Dear ImGui if requested (rarely used, only when ImGuiConfigFlags_NavEnableSetMousePos is enabled by user)
        if (io.WantSetMousePos)
          XWarpPointer(bd->display, None, *bd->window, 0, 0, 0, 0, io.MousePos.x, io.MousePos.y);

        // (Optional) Fallback to provide mouse position when focused (SDL_MOUSEMOTION already provides this when hovered or captured)
    
        // TODO:
    
        // if (bd->MouseCanUseGlobalState && bd->MouseButtonsDown == 0)
        // {
        //     int window_x, window_y, mouse_x_global, mouse_y_global;
        //     SDL_GetGlobalMouseState(&mouse_x_global, &mouse_y_global);
        //     SDL_GetWindowPosition(bd->Window, &window_x, &window_y);
        //     io.AddMousePosEvent((float)(mouse_x_global - window_x), (float)(mouse_y_global - window_y));
        // }
    }
}

static void ImGui_ImplX11_UpdateMouseCursor()
{
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange)
        return;
    ImGui_ImplX11_Data* bd = ImGui_ImplX11_GetBackendData();
   
    ImGuiMouseCursor imgui_cursor = ImGui::GetMouseCursor();
    if (io.MouseDrawCursor || imgui_cursor == ImGuiMouseCursor_None)
    {
        (void)bd;
        // Hide OS mouse cursor if imgui is drawing it or if it wants no cursor
        // SDL_ShowCursor(SDL_FALSE);
    }
    else
    {
        // Show OS mouse cursor
        // SDL_SetCursor(bd->MouseCursors[imgui_cursor] ? bd->MouseCursors[imgui_cursor] : bd->MouseCursors[ImGuiMouseCursor_Arrow]);
        // SDL_ShowCursor(SDL_TRUE);
    }
}

/*
static void ImGui_ImplX11_UpdateGamepads()
{
    ImGuiIO& io = ImGui::GetIO();
    if ((io.ConfigFlags & ImGuiConfigFlags_NavEnableGamepad) == 0)
        return;

    // Get gamepad
    io.BackendFlags &= ~ImGuiBackendFlags_HasGamepad;
    SDL_GameController* game_controller = SDL_GameControllerOpen(0);
    if (!game_controller)
        return;
    io.BackendFlags |= ImGuiBackendFlags_HasGamepad;

    // Update gamepad inputs
    #define IM_SATURATE(V)                      (V < 0.0f ? 0.0f : V > 1.0f ? 1.0f : V)
    #define MAP_BUTTON(KEY_NO, BUTTON_NO)       { io.AddKeyEvent(KEY_NO, SDL_GameControllerGetButton(game_controller, BUTTON_NO) != 0); }
    #define MAP_ANALOG(KEY_NO, AXIS_NO, V0, V1) { float vn = (float)(SDL_GameControllerGetAxis(game_controller, AXIS_NO) - V0) / (float)(V1 - V0); vn = IM_SATURATE(vn); io.AddKeyAnalogEvent(KEY_NO, vn > 0.1f, vn); }
    const int thumb_dead_zone = 8000;           // SDL_gamecontroller.h suggests using this value.
    MAP_BUTTON(ImGuiKey_GamepadStart,           SDL_CONTROLLER_BUTTON_START);
    MAP_BUTTON(ImGuiKey_GamepadBack,            SDL_CONTROLLER_BUTTON_BACK);
    MAP_BUTTON(ImGuiKey_GamepadFaceDown,        SDL_CONTROLLER_BUTTON_A);              // Xbox A, PS Cross
    MAP_BUTTON(ImGuiKey_GamepadFaceRight,       SDL_CONTROLLER_BUTTON_B);              // Xbox B, PS Circle
    MAP_BUTTON(ImGuiKey_GamepadFaceLeft,        SDL_CONTROLLER_BUTTON_X);              // Xbox X, PS Square
    MAP_BUTTON(ImGuiKey_GamepadFaceUp,          SDL_CONTROLLER_BUTTON_Y);              // Xbox Y, PS Triangle
    MAP_BUTTON(ImGuiKey_GamepadDpadLeft,        SDL_CONTROLLER_BUTTON_DPAD_LEFT);
    MAP_BUTTON(ImGuiKey_GamepadDpadRight,       SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
    MAP_BUTTON(ImGuiKey_GamepadDpadUp,          SDL_CONTROLLER_BUTTON_DPAD_UP);
    MAP_BUTTON(ImGuiKey_GamepadDpadDown,        SDL_CONTROLLER_BUTTON_DPAD_DOWN);
    MAP_BUTTON(ImGuiKey_GamepadL1,              SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
    MAP_BUTTON(ImGuiKey_GamepadR1,              SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);
    MAP_ANALOG(ImGuiKey_GamepadL2,              SDL_CONTROLLER_AXIS_TRIGGERLEFT,  0.0f, 32767);
    MAP_ANALOG(ImGuiKey_GamepadR2,              SDL_CONTROLLER_AXIS_TRIGGERRIGHT, 0.0f, 32767);
    MAP_BUTTON(ImGuiKey_GamepadL3,              SDL_CONTROLLER_BUTTON_LEFTSTICK);
    MAP_BUTTON(ImGuiKey_GamepadR3,              SDL_CONTROLLER_BUTTON_RIGHTSTICK);
    MAP_ANALOG(ImGuiKey_GamepadLStickLeft,      SDL_CONTROLLER_AXIS_LEFTX,  -thumb_dead_zone, -32768);
    MAP_ANALOG(ImGuiKey_GamepadLStickRight,     SDL_CONTROLLER_AXIS_LEFTX,  +thumb_dead_zone, +32767);
    MAP_ANALOG(ImGuiKey_GamepadLStickUp,        SDL_CONTROLLER_AXIS_LEFTY,  -thumb_dead_zone, -32768);
    MAP_ANALOG(ImGuiKey_GamepadLStickDown,      SDL_CONTROLLER_AXIS_LEFTY,  +thumb_dead_zone, +32767);
    MAP_ANALOG(ImGuiKey_GamepadRStickLeft,      SDL_CONTROLLER_AXIS_RIGHTX, -thumb_dead_zone, -32768);
    MAP_ANALOG(ImGuiKey_GamepadRStickRight,     SDL_CONTROLLER_AXIS_RIGHTX, +thumb_dead_zone, +32767);
    MAP_ANALOG(ImGuiKey_GamepadRStickUp,        SDL_CONTROLLER_AXIS_RIGHTY, -thumb_dead_zone, -32768);
    MAP_ANALOG(ImGuiKey_GamepadRStickDown,      SDL_CONTROLLER_AXIS_RIGHTY, +thumb_dead_zone, +32767);
    #undef MAP_BUTTON
    #undef MAP_ANALOG
}
*/

void ImGui_ImplX11_NewFrame(int w, int h, int display_w, int display_h)
{
    ImGui_ImplX11_Data* bd = ImGui_ImplX11_GetBackendData();
    IM_ASSERT(bd != NULL && "Did you call ImGui_ImplSDL2_Init()?");
    ImGuiIO& io = ImGui::GetIO();

    // Setup display size (every frame to accommodate for window resizing)
    io.DisplaySize = ImVec2((float)w, (float)h);
    if (w > 0 && h > 0)
        io.DisplayFramebufferScale = ImVec2((float)display_w / w, (float)display_h / h);

    // Setup time step (we don't use SDL_GetTicks() because it is using millisecond resolution)
    // static uint64_t frequency = SDL_GetPerformanceFrequency();
    auto current_time = std::chrono::high_resolution_clock::now();
    io.DeltaTime = (float)((current_time - bd->Time).count()) *  1000000000.0;
    bd->Time = current_time;

    if (bd->PendingMouseLeaveFrame && bd->PendingMouseLeaveFrame >= ImGui::GetFrameCount() && bd->MouseButtonsDown == 0)
    {
        io.AddMousePosEvent(-FLT_MAX, -FLT_MAX);
        bd->PendingMouseLeaveFrame = 0;
    }

    ImGui_ImplX11_UpdateMouseData();
    ImGui_ImplX11_UpdateMouseCursor();

    // Update game controllers (if enabled and available)
    // ImGui_ImplX11_UpdateGamepads();
}
