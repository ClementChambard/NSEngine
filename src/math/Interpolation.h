#ifndef INTERPOLATOR_IMPL_H
#define INTERPOLATOR_IMPL_H

#include <functional>
#include "./math.h"
#include "../defines.h"

namespace ns {

template<typename T>
extern T interpolateFunc(T start, T finish, u16 duration, u16 time, std::function<f32(f32)> funcgetval);

inline std::function<f32(f32)> flipFunc(f32 (*func)(f32))
{
    return ([func](f32 x) { return 1-(func(1-x)); });
}
inline std::function<f32(f32)> joinFlippedFunc(f32 (*func)(f32))
{
    return ([func](f32 x) {
        if (x < 0.5) return 0.5f*(func(2*x));
        else return 0.5f+0.5f*(flipFunc(func)(2*x-1));
    });
}
inline std::function<f32(f32)> joinFlippedFuncI(f32 (*func)(f32))
{
    return ([func](f32 x) {
        if (x < 0.5) return 0.5f*(flipFunc(func)(2*x));
        else return 0.5f+0.5f*(func(2*x-1));
    });
}

inline std::function<f32(f32)> FuncGetVal(u8 mode)
{
    switch (mode)
    {
        case 0:
            return [](f32 t) {return t;};
            break;
        case 1:
            return [](f32 t) {return t*t;};
            break;
        case 2:
            return [](f32 t) {return t*t*t;};
            break;
        case 3:
            return [](f32 t) {return t*t*t*t;};
            break;
        case 4:
            return flipFunc([](f32 t) {return t*t;});
            break;
        case 5:
            return flipFunc([](f32 t) {return t*t*t;});
            break;
        case 6:
            return flipFunc([](f32 t) {return t*t*t*t;});
            break;
        case 8:
            return [](f32 t) {return 3*t*t+2*t*t*t;};
            break;
        case 9:
            return joinFlippedFunc([](f32 t) {return t*t;});
            break;
        case 10:
            return joinFlippedFunc([](f32 t) {return t*t*t;});
            break;
        case 11:
            return joinFlippedFunc([](f32 t) {return t*t*t*t;});
            break;
        case 12:
            return joinFlippedFuncI([](f32 t) {return t*t;});
            break;
        case 13:
            return joinFlippedFuncI([](f32 t) {return t*t*t;});
            break;
        case 14:
            return joinFlippedFuncI([](f32 t) {return t*t*t*t;});
            break;
        case 15:
            return [](f32 t) {return t>=1?1:0;};
            break;
        case 16:
            return [](f32 t) {return t>0?1:0;};
            break;
        case 18:
            return [](f32 t) {return sin(t*PI/2.f);};
            break;
        case 19:
            return flipFunc([](f32 t) {return sin(t*PI/2.f);});
            break;
        case 20:
            return joinFlippedFuncI([](f32 t) {return sin(t*PI/2.f);});
            break;
        case 21:
            return joinFlippedFunc([](f32 t) {return sin(t*PI/2.f);});
            break;
        case 22:
            return [](f32 t) {return ((pow(t - 0.25, 2) / 0.5625) - 0.111111) / 0.888889;};
            break;
        case 23:
            return [](f32 t) {return ((pow(t - 0.3, 2) / 0.49) - 0.183673) / 0.816326;};
            break;
        case 24:
            return [](f32 t) {return ((pow(t - 0.35, 2) / 0.4225) - 0.289941) / 0.710059;};
            break;
        case 25:
            return [](f32 t) {return ((pow(t - 0.38, 2) / 0.3844) - 0.37565) / 0.62435;};
            break;
        case 26:
            return [](f32 t) {return ((pow(t - 0.4, 2) / 0.36) - 0.444444) / 0.555556;};
            break;
        case 27:
            return flipFunc([](f32 t) {return (f32)((pow(t - 0.25, 2) / 0.5625) - 0.111111) / 0.888889f;});
            break;
        case 28:
            return flipFunc([](f32 t) {return (f32)((pow(t - 0.3, 2) / 0.49) - 0.183673) / 0.816326f;});
            break;
        case 29:
            return flipFunc([](f32 t) {return (f32)((pow(t - 0.35, 2) / 0.4225) - 0.289941) / 0.710059f;});
            break;
        case 30:
            return flipFunc([](f32 t) {return (f32)((pow(t - 0.38, 2) / 0.3844) - 0.37565) / 0.62435f;});
            break;
        case 31:
            return flipFunc([](f32 t) {return (f32)((pow(t - 0.4f, 2) / 0.36f) - 0.444444f) / 0.555556f;});
            break;
        default:
            return [](f32 t) { return t; };
    }
}

template<typename T>
inline T interpolate(u8 mode, T start, T finish, u16 duration, u16 frame)
{
    return interpolateFunc(start, finish, duration, frame, FuncGetVal(mode));
}

}

#endif
