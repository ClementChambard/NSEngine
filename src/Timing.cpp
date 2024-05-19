#include "Timing.h"
#include "Error.h"

namespace ns {

FpsLimiter::FpsLimiter() {}

void FpsLimiter::begin() { begin_ticks = SDL_GetTicks(); }

f32 FpsLimiter::end() {
  i32 frameTicks = SDL_GetTicks() - begin_ticks;
  if (frameTicks < 1000 / 60)
    SDL_Delay(1000 / 60 - frameTicks);

  f32 frameTime = SDL_GetTicks() - begin_ticks;

  calculateFPS(frameTime);

  return static_cast<i32>(1000 / frameTime);
}

void FpsLimiter::setMaxFps(i32 maxfps) {
  maxFPS = maxfps;
  info("max FPS set to", maxfps);
}

void FpsLimiter::DrawFPS() const {
  // currentTexSwap = 0;
}

f32 FpsLimiter::GetFPS() const { return currentFps; }

f32 FpsLimiter::calculateFPS(f32 frameTime) {
  static const i32 NUM_SAMPLES = 20;
  static f32 frameTimes[NUM_SAMPLES];
  static i32 currentFrame = 1;
  frameTimes[currentFrame % NUM_SAMPLES] = frameTime;

  if (currentFrame == 0) {
    f32 tim = 0;
    for (i32 i = 0; i < NUM_SAMPLES; i++)
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
