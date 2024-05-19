#ifndef TIMINGDOTH
#define TIMINGDOTH

#include <SDL2/SDL.h>
#include "./defines.h"

namespace ns {

class FpsLimiter {

    public:
        FpsLimiter();

        void setMaxFps(i32 maxfps);

        void begin();
        f32 end();

        void DrawFPS() const;
        f32 GetFPS() const;

        i32 currentTexSwap = 0;

    private:
        i32 maxFPS;
        f32 currentFps = 0.f;
        u32 begin_ticks;
        f32 calculateFPS(f32 frameTimes);
};

}

#endif
