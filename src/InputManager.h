#ifndef INPUTMANAGER
#define INPUTMANAGER

#include <unordered_map>
#include <vector>
#include "Keys.h"
#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include "EventProcessor.h"
#include "./defines.h"

namespace ns {

struct KeyboardStruct;
struct MouseStruct;
struct keydata {
    bool down = false;
    bool pressed = false;
    bool released = false;
    void Press();
    void Release();
};

class InputManager {
    public:
        static void CheckEvents(SDL_Event& event);
        static void UpdateKeys();
        static bool KeyUp(u32 key);
        static bool KeyDown(u32 key);
        static bool KeyPressed(u32 key);
        static bool KeyReleased(u32 key);
        static void SetAsEventProcessor();

        static KeyboardStruct keyboard;
        static MouseStruct mouse;

    private:
        static std::unordered_map<u32, keydata> keyMap;
        class EventProcessor : public IEventProcessor {
            public: void ProcessEvent(SDL_Event* e, bool& noKeyboard, bool& noMouse) override;
            friend class InputManager;
        };
        static bool usingProcessor;
        static EventProcessor processor;
        friend class EventProcessor;
};

struct KeyboardStruct {
    bool Up(u32 key) const { return InputManager::KeyUp(key); }
    bool Down(u32 key) const { return InputManager::KeyDown(key); }
    bool Pressed(u32 key) const { return InputManager::KeyPressed(key); }
    bool Released(u32 key) const { return InputManager::KeyReleased(key); }
    int8_t Axis(u32 keyneg, u32 keypos) const { return Down(keypos) - Down(keyneg); }
};

struct MouseStruct {
    bool Down(u32 key) const { return InputManager::KeyDown(key); }
    bool Up(u32 key) const { return InputManager::KeyUp(key); }
    bool Pressed(u32 key) const { return InputManager::KeyPressed(key); }
    bool Released(u32 key) const { return InputManager::KeyReleased(key); }
    void Hide() const { SDL_ShowCursor(SDL_FALSE); }
    void Show() const { SDL_ShowCursor(SDL_TRUE); }
    void SetPos(i32 x, i32 y);
    void SetPos(glm::vec2 p) { SetPos(p.x, p.y); }
    void SetCursor(usize i);
    bool IsOffScreen();
    glm::vec2 pos;
    glm::vec2 guiPos;
    glm::vec2 posDiff;
    bool scrollUp = false;
    bool scrollDown = false;
    static std::vector<SDL_Cursor*> cursors;
};

struct GamepadStruct {
    static std::vector<GamepadStruct> gamepads;
    static int Number();
    static void Clear();
    bool Up(u32 button) const { return !buttons[button].down; }
    bool Down(u32 button) const { return buttons[button].down; }
    bool Pressed(u32 button) const { return buttons[button].pressed; }
    bool Released(u32 button) const { return buttons[button].released; }
    i16 AxisValue(i32 axis) const { return this->axis[axis]; }
    SDL_Joystick* joystick;
    SDL_GameController* controller;
    std::vector<keydata> buttons;
    std::vector<i16> axis;
};

}

namespace Inputs {

    extern void InitGamepads();
    inline ns::KeyboardStruct Keyboard() { return ns::InputManager::keyboard; }
    inline ns::MouseStruct Mouse() { return ns::InputManager::mouse; }
    inline ns::GamepadStruct Gamepad(i32 i) { return ns::GamepadStruct::gamepads[i]; }
}

#endif
