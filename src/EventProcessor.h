#ifndef EVENTPROCESSOR_H_
#define EVENTPROCESSOR_H_

#include <SDL2/SDL.h>

namespace NSEngine {

    class EventProcessor {
        public:
            EventProcessor() {}
            ~EventProcessor() {}

            virtual void ProcessEvent(SDL_Event* event, bool& noKeyboard, bool& noMouse) {}
    };

}

#endif // EVENTPROCESSOR_H_
