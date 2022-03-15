#ifndef TIMINGDOTH
#define TIMINGDOTH

#include <SDL2/SDL.h>

namespace NSEngine {

    class FpsLimiter {

        public:
            FpsLimiter();

            void setMaxFps(int maxfps) { maxFPS = maxfps; }

            void begin();
            float end();

            void DrawFPS();
            float GetFPS();

            int currentTexSwap = 0;

        private:
            int maxFPS;
            float currentFps = 0.f;
            unsigned int begin_ticks;
            float calculateFPS(float frameTimes);
    };

}

#endif
