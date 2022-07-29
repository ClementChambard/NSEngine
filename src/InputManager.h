#ifndef INPUTMANAGER
#define INPUTMANAGER

#include <unordered_map>
#include <vector>
#include "Keys.h"
#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include "EventProcessor.h"

namespace NSEngine {

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
            static void CheckEvents();
            static void UpdateKeys();
            static bool KeyUp(unsigned int key);
            static bool KeyDown(unsigned int key);
            static bool KeyPressed(unsigned int key);
            static bool KeyReleased(unsigned int key);
            static void SetAsEventProcessor();

            static KeyboardStruct keyboard;
            static MouseStruct mouse;

        private:
            static std::unordered_map<unsigned int, keydata> keyMap;
            static std::vector<keydata*> toUpdate;
            class _EventProcessor : public EventProcessor {
                public: void ProcessEvent(SDL_Event* e, bool& noKeyboard, bool& noMouse) override;
                friend class InputManager;
            };
            static bool usingProcessor;
            static _EventProcessor* processor;
            friend class _EventProcessor;
    };

    struct KeyboardStruct {
        bool Up(unsigned int key) const { return InputManager::KeyUp(key); }
        bool Down(unsigned int key) const { return InputManager::KeyDown(key); }
        bool Pressed(unsigned int key) const { return InputManager::KeyPressed(key); }
        bool Released(unsigned int key) const { return InputManager::KeyReleased(key); }
        int8_t Axis(unsigned int keyneg, unsigned int keypos) const { return Down(keypos) - Down(keyneg); }
    };

    struct MouseStruct {
        bool Down(unsigned int key) const { return InputManager::KeyDown(key); }
        bool Up(unsigned int key) const { return InputManager::KeyUp(key); }
        bool Pressed(unsigned int key) const { return InputManager::KeyPressed(key); }
        bool Released(unsigned int key) const { return InputManager::KeyReleased(key); }
        void Hide() const { SDL_ShowCursor(SDL_FALSE); }
        void Show() const { SDL_ShowCursor(SDL_TRUE); }
        void SetPos(int x, int y);
        void SetPos(const glm::vec2& p) { SetPos(p.x, p.y); }
        void SetCursor(int i);
        bool IsOffScreen();
        glm::vec2 pos;
        glm::vec2 guiPos;
        glm::vec2 posDiff;
        bool scrollUp = false;
        bool scrollDown = false;
        std::vector<SDL_Cursor*> cursors;
    };

    struct GamepadStruct {
        static std::vector<GamepadStruct> gamepads;
        static int Number();
        static void Clear();
        bool Up(unsigned int button) const { return !buttons[button].down; }
        bool Down(unsigned int button) const { return buttons[button].down; }
        bool Pressed(unsigned int button) const { return buttons[button].pressed; }
        bool Released(unsigned int button) const { return buttons[button].released; }
        int16_t AxisValue(int axis) const { return this->axis[axis]; }
        SDL_Joystick* joystick;
        SDL_GameController* controller;
        std::vector<keydata> buttons;
        std::vector<int16_t> axis;
    };

}

namespace Inputs {

    extern void InitGamepads();
    inline NSEngine::KeyboardStruct Keyboard() { return NSEngine::InputManager::keyboard; }
    inline NSEngine::MouseStruct Mouse() { return NSEngine::InputManager::mouse; }
    inline NSEngine::GamepadStruct Gamepad(int i) { return NSEngine::GamepadStruct::gamepads[i]; }
}

#endif
