#include "InputManager.h"
#include "NSEngine.h"
#include <vector>

namespace NSEngine {

    std::unordered_map<unsigned int, keydata> InputManager::keyMap;
    std::vector<keydata*> InputManager::toUpdate;
    std::vector<GamepadStruct> GamepadStruct::gamepads;
    KeyboardStruct InputManager::keyboard;
    MouseStruct InputManager::mouse;

    void InputManager::UpdateKeys()
    {
        while (!toUpdate.empty())
        {
            keydata* k = toUpdate.back();
            k->pressed = 0;
            k->released = 0;
            toUpdate.pop_back();
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

    void InputManager::CheckEvents()
    {
        if (engineData::event.type == SDL_KEYDOWN)
        {
            keyMap[engineData::event.key.keysym.sym].Press();
            toUpdate.push_back(&(keyMap[engineData::event.key.keysym.sym]));
        }
        if (engineData::event.type == SDL_KEYUP)
        {
            keyMap[engineData::event.key.keysym.sym].Release();
            toUpdate.push_back(&(keyMap[engineData::event.key.keysym.sym]));
        }
        if (engineData::event.type == SDL_JOYBUTTONDOWN || engineData::event.type == SDL_JOYBUTTONUP)
        {
            Uint8 j = engineData::event.jbutton.which;
            Uint8 button = engineData::event.jbutton.button;
            Uint8 state = engineData::event.jbutton.state;

            if (state == SDL_PRESSED) GamepadStruct::gamepads[j].buttons[button].Press();
            else if (state == SDL_RELEASED) GamepadStruct::gamepads[j].buttons[button].Release();
            toUpdate.push_back(&(GamepadStruct::gamepads[j].buttons[button]));
        }
        if (engineData::event.type == SDL_JOYAXISMOTION)
        {
            Uint8 j = engineData::event.jaxis.which;
            Uint8 axis = engineData::event.jaxis.axis;
            Sint16 val = engineData::event.jaxis.value;
            GamepadStruct::gamepads[j].axis[axis] = val;
        }
        if (engineData::event.type == SDL_MOUSEWHEEL)
        {
            if (engineData::event.wheel.y > 0) mouse.scrollUp = true;
            else if (engineData::event.wheel.y < 0) mouse.scrollDown = true;
        }
        if (engineData::event.type == SDL_MOUSEMOTION)
        {
            glm::vec2 newpos = {
                engineData::event.motion.x*engineData::displayRatio - engineData::gameWidth/2, 
                -engineData::event.motion.y*engineData::displayRatio + engineData::gameHeight/2
            };
            mouse.posDiff = newpos - mouse.guiPos;
            mouse.guiPos = newpos;
            if (NSEngine::activeCamera() != nullptr)
                mouse.pos = mouse.guiPos*NSEngine::activeCamera()->getScale() + glm::vec2(NSEngine::activeCamera()->getPosition().x, NSEngine::activeCamera()->getPosition().y);
            else mouse.pos = mouse.guiPos;
        }
        if (engineData::event.type == SDL_MOUSEBUTTONDOWN)
        {
            keyMap[engineData::event.button.button] = {
                1, 
                !keyMap[engineData::event.button.button].down, 
                keyMap[engineData::event.button.button].released
            };
            toUpdate.push_back(&(keyMap[engineData::event.button.button]));
        }
        if (engineData::event.type == SDL_MOUSEBUTTONUP)
        {
            keyMap[engineData::event.button.button] = {
                0, 
                keyMap[engineData::event.button.button].pressed, 
                keyMap[engineData::event.button.button].down
            };
            toUpdate.push_back(&(keyMap[engineData::event.button.button]));
        }
    }


    bool InputManager::KeyUp(unsigned int key)
    {
        return !keyMap[key].down;
    }

    bool InputManager::KeyDown(unsigned int key)
    {
        return keyMap[key].down;
    }

    bool InputManager::KeyPressed(unsigned int key)
    {
        return keyMap[key].pressed;
    }

    bool InputManager::KeyReleased(unsigned int key)
    {
        return keyMap[key].released;
    }

    void MouseStruct::SetPos(int x, int y)
    {
        SDL_WarpMouseInWindow(engineData::window, (int)((x+engineData::gameWidth/2)/engineData::displayRatio), (int)((-y+engineData::gameHeight/2)/engineData::displayRatio));
    }

    bool MouseStruct::IsOffScreen()
    {
        return abs(guiPos.x) >= engineData::gameWidth/2 || abs(guiPos.y) >= engineData::gameHeight/2;
    }

    int GamepadStruct::Number()
    {
        return GamepadStruct::gamepads.size();
    }
    
    void GamepadStruct::Clear()
    {
        for (int i = 0; i < gamepads.size(); i++)
            SDL_JoystickClose(gamepads[i].joystick);
        gamepads.clear();
    }
}

void Inputs::InitGamepads()
{
    NSEngine::GamepadStruct::Clear();
    int numJoystick = SDL_NumJoysticks();
    for (int i = 0; i < numJoystick; i++)
    {
        NSEngine::info("opened joystick");
        SDL_Joystick* j = SDL_JoystickOpen(i);
        NSEngine::GamepadStruct::gamepads.push_back(NSEngine::GamepadStruct());
        NSEngine::GamepadStruct::gamepads.back().joystick = j;
        NSEngine::GamepadStruct::gamepads.back().buttons.resize(SDL_JoystickNumButtons(j));
        NSEngine::GamepadStruct::gamepads.back().axis.resize(SDL_JoystickNumAxes(j));
    }
}
