#include "InputManager.h"
#include "NSEngine.h"
#include <vector>

namespace NSEngine {

    std::unordered_map<unsigned int, keydata> InputManager::keyMap;
    std::vector<keydata*> InputManager::toUpdate;
    std::vector<GamepadStruct> GamepadStruct::gamepads;
    KeyboardStruct InputManager::keyboard;
    MouseStruct InputManager::mouse;
    bool InputManager::usingProcessor = false;
    InputManager::_EventProcessor* InputManager::processor = new InputManager::_EventProcessor();

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
        bool FALSEBOOL = false;
        if (!usingProcessor) processor->ProcessEvent(&engineData::event, FALSEBOOL, FALSEBOOL);
    }

    void InputManager::SetAsEventProcessor()
    {
        usingProcessor = true;
        engineData::eventProcessors.push_back(processor);
    }

    void InputManager::_EventProcessor::ProcessEvent(SDL_Event* e, bool& noKeyboard, bool& noMouse)
    {
        if (e->type == SDL_KEYDOWN)
        {
            if (noKeyboard) return;
            keyMap[e->key.keysym.sym].Press();
            toUpdate.push_back(&(keyMap[e->key.keysym.sym]));
        }
        if (e->type == SDL_KEYUP)
        {
            if (noKeyboard) return;
            keyMap[e->key.keysym.sym].Release();
            toUpdate.push_back(&(keyMap[e->key.keysym.sym]));
        }
        if (e->type == SDL_JOYBUTTONDOWN || e->type == SDL_JOYBUTTONUP)
        {
            Uint8 j = e->jbutton.which;
            if (!GamepadStruct::gamepads[j].controller)
            {
                Uint8 button = e->jbutton.button;
                Uint8 state = e->jbutton.state;
                //std::cout << (int)j << " : " << (int)button << " -> " << (int)state << "\n";
                if (state == SDL_PRESSED) GamepadStruct::gamepads[j].buttons[button].Press();
                else if (state == SDL_RELEASED) GamepadStruct::gamepads[j].buttons[button].Release();
                toUpdate.push_back(&(GamepadStruct::gamepads[j].buttons[button]));
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
            if (noMouse) return;
            if (e->wheel.y > 0) mouse.scrollUp = true;
            else if (e->wheel.y < 0) mouse.scrollDown = true;
        }
        if (e->type == SDL_MOUSEMOTION)
        {
            if (noMouse) return;
            glm::vec2 newpos = {
                e->motion.x*engineData::displayRatio - engineData::gameWidth/2,
                -e->motion.y*engineData::displayRatio + engineData::gameHeight/2
            };
            mouse.posDiff = newpos - mouse.guiPos;
            mouse.guiPos = newpos;
            if (NSEngine::activeCamera() != nullptr)
                mouse.pos = mouse.guiPos*NSEngine::activeCamera()->getScale() + glm::vec2(NSEngine::activeCamera()->getPosition().x, NSEngine::activeCamera()->getPosition().y);
            else mouse.pos = mouse.guiPos;
        }
        if (e->type == SDL_MOUSEBUTTONDOWN)
        {
            if (noMouse) return;
            keyMap[e->button.button] = {
                1, 
                !keyMap[e->button.button].down,
                keyMap[e->button.button].released
            };
            toUpdate.push_back(&(keyMap[e->button.button]));
        }
        if (e->type == SDL_MOUSEBUTTONUP)
        {
            if (noMouse) return;
            keyMap[e->button.button] = {
                0, 
                keyMap[e->button.button].pressed,
                keyMap[e->button.button].down
            };
            toUpdate.push_back(&(keyMap[e->button.button]));
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

    void MouseStruct::SetCursor(int i)
    {
        if (cursors.empty())
        {
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
        if (i >= 0 && i < cursors.size()) SDL_SetCursor(cursors[i]);
    }

    int GamepadStruct::Number()
    {
        return GamepadStruct::gamepads.size();
    }
    
    void GamepadStruct::Clear()
    {
        for (int i = 0; i < gamepads.size(); i++)
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
    NSEngine::GamepadStruct::Clear();
    int numJoystick = SDL_NumJoysticks();
    for (int i = 0; i < numJoystick; i++)
    {
        SDL_Joystick* j = SDL_JoystickOpen(i);
        SDL_GameController* c = nullptr;
        if (SDL_IsGameController(i)) {
               c = SDL_GameControllerOpen(i);
               NSEngine::info("opened controller : ", SDL_GameControllerName(c));
        } else NSEngine::info("opened joystick");
        NSEngine::GamepadStruct::gamepads.push_back(NSEngine::GamepadStruct());
        NSEngine::GamepadStruct::gamepads.back().joystick = j;
        NSEngine::GamepadStruct::gamepads.back().controller = c;
        NSEngine::GamepadStruct::gamepads.back().buttons.resize(SDL_JoystickNumButtons(j));
        NSEngine::GamepadStruct::gamepads.back().axis.resize(SDL_JoystickNumAxes(j));
    }
}
