#include "InputManager.h"
#include "NSEngine.hpp"
#include "./logger.h"
#include <vector>
#include "platform/platform_wnd.h"

namespace ns {

std::unordered_map<u32, keydata> InputManager::keyMap;
std::vector<GamepadStruct> GamepadStruct::gamepads;
KeyboardStruct InputManager::keyboard;
MouseStruct InputManager::mouse;
bool InputManager::usingProcessor = false;
InputManager::EventProcessor InputManager::processor;

void InputManager::UpdateKeys()
{
    for (auto& [_, k] : keyMap)
    {
        k.pressed = 0;
        k.released = 0;
    }
    mouse.scrollDown = false;
    mouse.scrollUp = false;
    mouse.posDiff = {0, 0};
}

void keydata::Press()
{
    pressed = !down;
    down = 1;
}
void keydata::Release()
{
    released = down;
    down = 0;
}

void InputManager::CheckEvents(SDL_Event& event)
{
    bool FALSEBOOL = false;
    if (!usingProcessor) processor.ProcessEvent(&event, FALSEBOOL, FALSEBOOL);
}

void InputManager::SetAsEventProcessor()
{
    usingProcessor = true;
    getInstance()->addEventProcessor(&processor);
}

void InputManager::EventProcessor::ProcessEvent(SDL_Event* e, bool& capture_keyboard, bool& capture_mouse)
{
    if (e->type == SDL_KEYDOWN)
    {
        if (capture_keyboard) return;
        keyMap[e->key.keysym.sym].Press();
    }
    if (e->type == SDL_KEYUP)
    {
        if (capture_keyboard) return;
        keyMap[e->key.keysym.sym].Release();
    }
    if (e->type == SDL_JOYBUTTONDOWN || e->type == SDL_JOYBUTTONUP)
    {
        Uint8 j = e->jbutton.which;
        if (!GamepadStruct::gamepads[j].controller)
        {
            Uint8 button = e->jbutton.button;
            Uint8 state = e->jbutton.state;
            if (state == SDL_PRESSED) GamepadStruct::gamepads[j].buttons[button].Press();
            else if (state == SDL_RELEASED) GamepadStruct::gamepads[j].buttons[button].Release();
        }
    }
    if (e->type == SDL_JOYAXISMOTION)
    {
        Uint8 j = e->jaxis.which;
        if (!GamepadStruct::gamepads[j].controller) {
            Uint8 axis = e->jaxis.axis;
            Sint16 val = e->jaxis.value;
            GamepadStruct::gamepads[j].axis[axis] = val;
        }
    }
    if (e->type == SDL_MOUSEWHEEL)
    {
        if (capture_mouse) return;
        if (e->wheel.y > 0) mouse.scrollUp = true;
        else if (e->wheel.y < 0) mouse.scrollDown = true;
    }
    if (e->type == SDL_MOUSEMOTION)
    {
        if (capture_mouse) return;
        auto windata = getInstance()->window().getWindowData();
        float displayRatio = windata.width/(float)windata.bwidth;
        glm::vec2 newpos = {
            (i32)(e->motion.x*displayRatio) - windata.bwidth/2,
            (i32)(-e->motion.y*displayRatio) + windata.bheight/2
        };
        mouse.posDiff = newpos - mouse.guiPos;
        mouse.guiPos = newpos;
        mouse.pos = mouse.guiPos;
    }
    if (e->type == SDL_MOUSEBUTTONDOWN)
    {
        if (capture_mouse) return;
        keyMap[e->button.button] = {
            1, 
            !keyMap[e->button.button].down,
            keyMap[e->button.button].released
        };
    }
    if (e->type == SDL_MOUSEBUTTONUP)
    {
        if (capture_mouse) return;
        keyMap[e->button.button] = {
            0, 
            keyMap[e->button.button].pressed,
            keyMap[e->button.button].down
        };
    }
}


bool InputManager::KeyUp(u32 key)
{
    return !keyMap[key].down;
}

bool InputManager::KeyDown(u32 key)
{
    return keyMap[key].down;
}

bool InputManager::KeyPressed(u32 key)
{
    return keyMap[key].pressed;
}

bool InputManager::KeyReleased(u32 key)
{
    return keyMap[key].released;
}

void MouseStruct::SetPos(i32 x, i32 y)
{
    auto windata = getInstance()->window().getWindowData();
    float displayRatio = windata.width/(float)windata.bwidth;
    platform::wnd_set_mouse_pos(getInstance()->window().getPlatformWindow(),
                          (i32)((i32)(x+windata.bwidth/2)/displayRatio),
                          (i32)((i32)(-y+windata.bheight/2)/displayRatio));
}

bool MouseStruct::IsOffScreen()
{
    auto windata = getInstance()->window().getWindowData();
    return abs(guiPos.x) >= (i32)(windata.bwidth/2) || abs(guiPos.y) >= (i32)(windata.bheight/2);
}

std::vector<SDL_Cursor*> MouseStruct::cursors;
void MouseStruct::SetCursor(usize i)
{
    static bool ok = false;
    if (!ok)
    {
        ok = true;
        cursors.push_back(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW));
        cursors.push_back(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM));
        cursors.push_back(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_WAIT));
        cursors.push_back(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_CROSSHAIR));
        cursors.push_back(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_WAITARROW));
        cursors.push_back(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENWSE));
        cursors.push_back(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENESW));
        cursors.push_back(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEWE));
        cursors.push_back(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENS));
        cursors.push_back(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEALL));
        cursors.push_back(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_NO));
        cursors.push_back(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND));
    }
    if (i < cursors.size()) SDL_SetCursor(cursors[i]);
}

i32 GamepadStruct::Number()
{
    return GamepadStruct::gamepads.size();
}

void GamepadStruct::Clear()
{
    for (size_t i = 0; i < gamepads.size(); i++)
    {
        SDL_JoystickClose(gamepads[i].joystick);
        if (gamepads[i].controller) SDL_GameControllerClose(gamepads[i].controller);
    }
    gamepads.clear();
}

}

void Inputs::InitGamepads()
{
    SDL_GameControllerAddMappingsFromFile("assets/engine/gamecontrollerdb.txt");
    ns::GamepadStruct::Clear();
    i32 numJoystick = SDL_NumJoysticks();
    for (i32 i = 0; i < numJoystick; i++)
    {
        SDL_Joystick* j = SDL_JoystickOpen(i);
        SDL_GameController* c = nullptr;
        if (SDL_IsGameController(i)) {
               c = SDL_GameControllerOpen(i);
               NS_INFO("opened controller : %s", SDL_GameControllerName(c));
        } else NS_INFO("opened joystick");
        ns::GamepadStruct::gamepads.push_back(ns::GamepadStruct());
        ns::GamepadStruct::gamepads.back().joystick = j;
        ns::GamepadStruct::gamepads.back().controller = c;
        ns::GamepadStruct::gamepads.back().buttons.resize(SDL_JoystickNumButtons(j));
        ns::GamepadStruct::gamepads.back().axis.resize(SDL_JoystickNumAxes(j));
    }
}
