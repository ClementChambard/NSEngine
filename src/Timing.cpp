#include "Timing.h"
#include "Assets.h"
#include "GraphicsLayer.h"
#include "NSEngine.h"
#include "SpriteFont.h"
#include "SpriteManager.h"

#include <iostream>
#include <string>

namespace NSEngine {

    FpsLimiter::FpsLimiter() {}

    void FpsLimiter::begin() 
    {
        begin_ticks = SDL_GetTicks();
    }

    float FpsLimiter::end() 
    {
        int frameTicks = SDL_GetTicks() - begin_ticks;
        if (frameTicks < 1000 / 60)
            SDL_Delay(1000 / 60 - frameTicks);

        float frameTime = SDL_GetTicks() - begin_ticks;

        calculateFPS(frameTime);

        return (int) (1000 / frameTime);
    }

    void FpsLimiter::DrawFPS()
    {
        //if (!(engineData::gameflags & 0b00000010)) return;
        //draw_set_layer(2);
        //draw_set_halign(fa_left);
        //draw_set_valign(fa_top);
        //draw_text(gameassets::fnt_base, ("FPS:" + std::to_string(currentFps)).c_str(), -engineData::gameWidth/2, engineData::gameHeight/2);
        //draw_text(gameassets::fnt_small, ("(" + std::to_string(currentTexSwap) + ")").c_str(), -engineData::gameWidth/2 + 80, engineData::gameHeight/2);
        currentTexSwap = 0;
    }
    float FpsLimiter::GetFPS()
    {
        return currentFps;
    }

    float FpsLimiter::calculateFPS(float frameTime)
    {
        static const int NUM_SAMPLES = 20;
        static float frameTimes[NUM_SAMPLES];
        static int currentFrame = 1;
        frameTimes[currentFrame % NUM_SAMPLES] = frameTime;

        if (currentFrame == 0)
        {
            float tim = 0, rtim = 0;
            for (int i = 0; i < NUM_SAMPLES; i++) tim += frameTimes[i];
            tim /= NUM_SAMPLES;
            std::to_string(tim);
            if (tim!=0)
            currentFps = 1000.f / tim;
            //std::cout << "FPS : " << FPS << std::endl;
        }
        currentFrame = (currentFrame+1) % NUM_SAMPLES;

        return currentFps;
    }

}
