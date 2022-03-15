#include "Profiler.h"
#include "Timing.h"
#include "math/math.h"
#include "NSEngine.h"

namespace NSEngine {

    int Profiler::currentSection = -1;
    int Profiler::x, Profiler::y, Profiler::r, Profiler::r2;
    bool Profiler::isInit = false;
    unsigned int Profiler::currentTicks = 0;
    std::vector<Profiler::section> Profiler::sections;

    void Profiler::Draw()
    {

        if (!isInit) return;
        
        draw_set_layer(engineData::debugLayer);       
        float totalTime = 0;
        for (section s : sections) totalTime += s.lastTime();
        if (totalTime == 0) return;
        float currentAngle = 0.f;
        for (section s : sections)
        {
            float a = PI2 * s.lastTime()/totalTime;
            draw_circle_arc_color(x,y,r2,r,currentAngle,currentAngle+a,s.c,s.c);
            currentAngle+=a;
        }

    }

}
