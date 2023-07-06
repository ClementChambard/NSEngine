#include "Timing.h"
#include "Error.h"

#include <iostream>
#include <string>

namespace NSEngine {

FpsLimiter::FpsLimiter() {}

void FpsLimiter::begin() { begin_ticks = SDL_GetTicks(); }

float FpsLimiter::end() {
  int frameTicks = SDL_GetTicks() - begin_ticks;
  if (frameTicks < 1000 / 60)
    SDL_Delay(1000 / 60 - frameTicks);

  float frameTime = SDL_GetTicks() - begin_ticks;

  calculateFPS(frameTime);

  return static_cast<int>(1000 / frameTime);
}

void FpsLimiter::setMaxFps(int maxfps) {
  maxFPS = maxfps;
  info("max FPS set to", maxfps);
}

void FpsLimiter::DrawFPS() const {
  // currentTexSwap = 0;
}

float FpsLimiter::GetFPS() const { return currentFps; }

float FpsLimiter::calculateFPS(float frameTime) {
  static const int NUM_SAMPLES = 20;
  static float frameTimes[NUM_SAMPLES];
  static int currentFrame = 1;
  frameTimes[currentFrame % NUM_SAMPLES] = frameTime;

  if (currentFrame == 0) {
    float tim = 0;
    for (int i = 0; i < NUM_SAMPLES; i++)
      tim += frameTimes[i];
    tim /= NUM_SAMPLES;
    // std::to_string(tim);
    if (tim != 0)
      currentFps = 1000.f / tim;
  }
  currentFrame = (currentFrame + 1) % NUM_SAMPLES;

  return currentFps;
}

} // namespace NSEngine
