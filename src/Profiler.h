#ifndef PROFILER
#define PROFILER

#include "vertex.h"
#include <vector>
#include <iostream>
#include <SDL2/SDL.h>
namespace NSEngine {

    class Profiler {
        public:


            static void Init(int x, int y, int r, int r2 = 0) { isInit = true; Profiler::x = x; Profiler::y = y; Profiler::r = r; Profiler::r2 = r2; }
            static void AddSection(Color c, std::string name) {sections.push_back({c,name}); }
            static void EndSection() { sections[currentSection].time(SDL_GetTicks()-currentTicks); }
            static void StartSection(int i) { currentSection = i; currentTicks = SDL_GetTicks(); }
            static void Draw();
        private:
            struct section
            {
                Color c = {255,255,255,255};
                std::string name;
                std::vector<unsigned int> lastTimes = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
                int pos = 0;
                float lastTime() { float t = 0; for (int i : lastTimes) t += i; return t/20.f; }
                void time(int i) { lastTimes[pos] = i; pos = (pos+1)%20; }
            };
            static bool isInit;
            static int x, y, r, r2;
            static int currentSection;
            static unsigned int currentTicks;
            static std::vector<section> sections;
    };

}
#endif
