#ifndef EVENTPROCESSOR_H_
#define EVENTPROCESSOR_H_

#include <SDL2/SDL.h>

namespace NSEngine {

    class IEventProcessor {
        public:
            virtual ~IEventProcessor() {};
            virtual void ProcessEvent(SDL_Event* event, bool& capture_keyboard, bool& capture_mouse) = 0;
    };

}

#endif // EVENTPROCESSOR_H_
