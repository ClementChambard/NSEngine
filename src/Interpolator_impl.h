#ifndef INTERPOLATOR_IMPL_H
#define INTERPOLATOR_IMPL_H

#include <iostream>
#include <functional>
#include "math/math.h"
#include <glm/glm.hpp>

namespace NSEngine {

template<typename T>
extern T interpolateFunc(T start, T finish, uint16_t duration, uint16_t time, std::function<float(float)> funcgetval);

inline std::function<float(float)> flipFunc(float (*func)(float))
{
    return ([func](float x) { return 1-(func(1-x)); });
}
inline std::function<float(float)> joinFlippedFunc(float (*func)(float))
{
    return ([func](float x) {
        if (x < 0.5) return 0.5f*(func(2*x));
        else return 0.5f+0.5f*(flipFunc(func)(2*x-1));
    });
}
inline std::function<float(float)> joinFlippedFuncI(float (*func)(float))
{
    return ([func](float x) {
        if (x < 0.5) return 0.5f*(flipFunc(func)(2*x));
        else return 0.5f+0.5f*(func(2*x-1));
    });
}

inline std::function<float(float)> FuncGetVal(uint8_t mode)
{
    switch (mode)
    {
        case 0:
            return [](float t) {return t;};
            break;
        case 1:
            return [](float t) {return t*t;};
            break;
        case 2:
            return [](float t) {return t*t*t;};
            break;
        case 3:
            return [](float t) {return t*t*t*t;};
            break;
        case 4:
            return flipFunc([](float t) {return t*t;});
            break;
        case 5:
            return flipFunc([](float t) {return t*t*t;});
            break;
        case 6:
            return flipFunc([](float t) {return t*t*t*t;});
            break;
        case 8:
            return [](float t) {return 3*t*t+2*t*t*t;};
            break;
        case 9:
            return joinFlippedFunc([](float t) {return t*t;});
            break;
        case 10:
            return joinFlippedFunc([](float t) {return t*t*t;});
            break;
        case 11:
            return joinFlippedFunc([](float t) {return t*t*t*t;});
            break;
        case 12:
            return joinFlippedFuncI([](float t) {return t*t;});
            break;
        case 13:
            return joinFlippedFuncI([](float t) {return t*t*t;});
            break;
        case 14:
            return joinFlippedFuncI([](float t) {return t*t*t*t;});
            break;
        case 15:
            return [](float t) {return t>=1?1:0;};
            break;
        case 16:
            return [](float t) {return t>0?1:0;};
            break;
        case 18:
            return [](float t) {return sin(t*PI/2.f);};
            break;
        case 19:
            return flipFunc([](float t) {return sin(t*PI/2.f);});
            break;
        case 20:
            return joinFlippedFuncI([](float t) {return sin(t*PI/2.f);});
            break;
        case 21:
            return joinFlippedFunc([](float t) {return sin(t*PI/2.f);});
            break;
        case 22:
            return [](float t) {return ((pow(t - 0.25, 2) / 0.5625) - 0.111111) / 0.888889;};
            break;
        case 23:
            return [](float t) {return ((pow(t - 0.3, 2) / 0.49) - 0.183673) / 0.816326;};
            break;
        case 24:
            return [](float t) {return ((pow(t - 0.35, 2) / 0.4225) - 0.289941) / 0.710059;};
            break;
        case 25:
            return [](float t) {return ((pow(t - 0.38, 2) / 0.3844) - 0.37565) / 0.62435;};
            break;
        case 26:
            return [](float t) {return ((pow(t - 0.4, 2) / 0.36) - 0.444444) / 0.555556;};
            break;
        case 27:
            return flipFunc([](float t) {return (float)((pow(t - 0.25, 2) / 0.5625) - 0.111111) / 0.888889f;});
            break;
        case 28:
            return flipFunc([](float t) {return (float)((pow(t - 0.3, 2) / 0.49) - 0.183673) / 0.816326f;});
            break;
        case 29:
            return flipFunc([](float t) {return (float)((pow(t - 0.35, 2) / 0.4225) - 0.289941) / 0.710059f;});
            break;
        case 30:
            return flipFunc([](float t) {return (float)((pow(t - 0.38, 2) / 0.3844) - 0.37565) / 0.62435f;});
            break;
        case 31:
            return flipFunc([](float t) {return (float)((pow(t - 0.4f, 2) / 0.36f) - 0.444444f) / 0.555556f;});
            break;
        default:
            return [](float t) { return t; };
    }
}

template<typename T>
inline T interpolate(uint8_t mode, T start, T finish, uint16_t duration, uint16_t frame)
{
    return interpolateFunc(start, finish, duration, frame, FuncGetVal(mode));

    /*
    switch (mode)
    {
        case 0:
            return interpolateFunc(start,finish,duration,frame,[](float x) {return x;});
            break;
        case 1:
            return interpolateFunc(start,finish,duration,frame,[](float x) {return x*x;});
            break;
        case 2:
            return interpolateFunc(start,finish,duration,frame,[](float x) {return x*x*x;});
            break;
        case 3:
            return interpolateFunc(start,finish,duration,frame,[](float x) {return x*x*x*x;});
            break;
        case 4:
            return interpolateFunc(start,finish,duration,frame,flipFunc([](float x) {return x*x;}));
            break;
        case 5:
            return interpolateFunc(start,finish,duration,frame,flipFunc([](float x) {return x*x*x;}));
            break;
        case 6:
            return interpolateFunc(start,finish,duration,frame,flipFunc([](float x) {return x*x*x*x;}));
            break;
        case 8:
            return interpolateFunc(start,finish,duration,frame,[](float x) {return 3*x*x+2*x*x*x;});
            break;
        case 9:
            return interpolateFunc(start,finish,duration,frame,joinFlippedFunc([](float x) {return x*x;}));
            break;
        case 10:
            return interpolateFunc(start,finish,duration,frame,joinFlippedFunc([](float x) {return x*x*x;}));
            break;
        case 11:
            return interpolateFunc(start,finish,duration,frame,joinFlippedFunc([](float x) {return x*x*x*x;}));
            break;
        case 12:
            return interpolateFunc(start,finish,duration,frame,joinFlippedFuncI([](float x) {return x*x;}));
            break;
        case 13:
            return interpolateFunc(start,finish,duration,frame,joinFlippedFuncI([](float x) {return x*x*x;}));
            break;
        case 14:
            return interpolateFunc(start,finish,duration,frame,joinFlippedFuncI([](float x) {return x*x*x*x;}));
            break;
        case 15:
            return frame>=duration?finish:start;
            break;
        case 16:
            return frame>0?finish:start;
            break;
        case 18:
            return interpolateFunc(start,finish,duration,frame,[](float x) {return sin(x*PI/2.f);});
            break;
        case 19:
            return interpolateFunc(start,finish,duration,frame,flipFunc([](float x) {return sin(x*PI/2.f);}));
            break;
        case 20:
            return interpolateFunc(start,finish,duration,frame,joinFlippedFuncI([](float x) {return sin(x*PI/2.f);}));
            break;
        case 21:
            return interpolateFunc(start,finish,duration,frame,joinFlippedFunc([](float x) {return sin(x*PI/2.f);}));
            break;
        case 22:
            return interpolateFunc(start,finish,duration,frame,[](float x) {return ((pow(x - 0.25, 2) / 0.5625) - 0.111111) / 0.888889;});
            break;
        case 23:
            return interpolateFunc(start,finish,duration,frame,[](float x) {return ((pow(x - 0.3, 2) / 0.49) - 0.183673) / 0.816326;});
            break;
        case 24:
            return interpolateFunc(start,finish,duration,frame,[](float x) {return ((pow(x - 0.35, 2) / 0.4225) - 0.289941) / 0.710059;});
            break;
        case 25:
            return interpolateFunc(start,finish,duration,frame,[](float x) {return ((pow(x - 0.38, 2) / 0.3844) - 0.37565) / 0.62435;});
            break;
        case 26:
            return interpolateFunc(start,finish,duration,frame,[](float x) {return ((pow(x - 0.4, 2) / 0.36) - 0.444444) / 0.555556;});
            break;
        case 27:
            return interpolateFunc(start,finish,duration,frame,flipFunc([](float x) {return (float)((pow(x - 0.25, 2) / 0.5625) - 0.111111) / 0.888889f;}));
            break;
        case 28:
            return interpolateFunc(start,finish,duration,frame,flipFunc([](float x) {return (float)((pow(x - 0.3, 2) / 0.49) - 0.183673) / 0.816326f;}));
            break;
        case 29:
            return interpolateFunc(start,finish,duration,frame,flipFunc([](float x) {return (float)((pow(x - 0.35, 2) / 0.4225) - 0.289941) / 0.710059f;}));
            break;
        case 30:
            return interpolateFunc(start,finish,duration,frame,flipFunc([](float x) {return (float)((pow(x - 0.38, 2) / 0.3844) - 0.37565) / 0.62435f;}));
            break;
        case 31:
            return interpolateFunc(start,finish,duration,frame,flipFunc([](float x) {return (float)((pow(x - 0.4f, 2) / 0.36f) - 0.444444f) / 0.555556f;}));
            break;
        default:
            return T();
    }
    */
}

}

#endif
