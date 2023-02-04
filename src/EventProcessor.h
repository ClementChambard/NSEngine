#ifndef EVENTPROCESSOR_H_
#define EVENTPROCESSOR_H_

#include <SDL2/SDL.h>

namespace NSEngine {

    class EventProcessor {
        public:
            virtual void ProcessEvent(SDL_Event* event, bool& noKeyboard, bool& noMouse) = 0;
    };

}

#endif // EVENTPROCESSOR_H_
