#ifndef INTERP_HEADER_INCLUDED
#define INTERP_HEADER_INCLUDED

#include "./defines.h"
#include "./Timer.hpp"
#include "./math/types/vec/vec3.hpp"

namespace ns {

using PFN_InterpFunc = f32 (*)(f32) ;

enum class InterpMethod : u8 {
    LINEAR = 0,
    EASEIN2,
    EASEIN3,
    EASEIN4,
    EASEOUT2,
    EASEOUT3,
    EASEOUT4,
    CSTSPEED,
    BEZIER,
    EASEINOUT2,
    EASEINOUT3,
    EASEINOUT4,
    EASEOUTIN2,
    EASEOUTIN3,
    EASEOUTIN4,
    _15,
    _16,
    CSTACCEL,
    // ... 
};

PFN_InterpFunc getInterpFunc(InterpMethod mode);

template<typename T>
struct Interp {
    T initial = T{};
    T goal = T{};
    T bezier_1 = T{};
    T bezier_2 = T{};
    T current = T{};
    Timer_t time = 0;
    i32 end_time = 0;
    InterpMethod method = InterpMethod::LINEAR;

    void start(T begin, T end, i32 time, i32 mode) {
        initial = begin;
        current = initial;
        goal = end;
        bezier_1 = T{};
        bezier_2 = T{};
        end_time = time;
        method = InterpMethod(mode);
        this->time = 0;
    }

    void start_ex(T begin, T end, T b1, T b2, i32 time, i32 mode) {
        initial = begin;
        current = initial;
        goal = end;
        bezier_1 = b1;
        bezier_2 = b2;
        end_time = time;
        method = InterpMethod(mode);
        this->time = 0;
    }

    void start_bezier(T begin, T end, T b1, T b2, i32 time) {
        initial = begin;
        current = initial;
        goal = end;
        bezier_1 = b1;
        bezier_2 = b2;
        end_time = time;
        method = InterpMethod::BEZIER;
        this->time = 0;
    }

    T step() {
        if (end_time != 0) {
            time++;
            if (time < end_time || end_time < 0) {
                if (method == InterpMethod::CSTSPEED) {
                    initial += goal;
                    current = initial;
                } else if (method == InterpMethod::CSTACCEL) {
                    initial += bezier_2;
                    bezier_2 += goal;
                    current = initial;
                } else if (method == InterpMethod::BEZIER) {
                    f32 x = time.current_f /
                              static_cast<f32>(end_time);
                    T a0 = initial;
                    T a1 = bezier_1;
                    T a2 = 3.f * (goal - initial) - 2.f * bezier_1 - bezier_2;
                    T a3 = 2.f * (initial - goal) + bezier_1 + bezier_2;
                    current = a0 + a1 * x + a2 * x * x + a3 * x * x * x;
                } else {
                    current = (goal - initial) * getInterpFunc(method)(
                        time.current_f / static_cast<f32>(end_time))
                        + initial;
                }
                return current;
            }
            time = end_time;
            end_time = 0;
        }
        if (method != InterpMethod::CSTSPEED && method != InterpMethod::CSTACCEL) {
            return goal;
        }
        return initial;
    }
};

struct InterpStrange {
    vec3 current;
    vec3 initial;
    vec3 goal;
    vec3 bezier_1;
    vec3 bezier_2;
    Timer_t time;
    i32 end_time;
    InterpMethod method_for_1d[3];
    InterpMethod method_for_3d;
    i32 flags;

    void start(vec3 begin, vec3 end, i32 time, i32 mode) {
        initial = begin;
        goal = end;
        bezier_1 = vec3();
        bezier_2 = vec3();
        end_time = time;
        method_for_3d = InterpMethod(mode);
        this->time = 0;
        flags = 0;
    }

    void start_curve(vec3 begin, vec3 end, i32 time,
                     i32 mode_x, i32 mode_y, i32 mode_z) {
        initial = begin;
        goal = end;
        bezier_1 = vec3();
        bezier_2 = vec3();
        end_time = time;
        method_for_1d[0] = InterpMethod(mode_x);
        method_for_1d[1] = InterpMethod(mode_y);
        method_for_1d[2] = InterpMethod(mode_z);
        this->time = 0;
        flags = 1;
    }

    void start_bezier(vec3 begin, vec3 end, vec3 b1,
                      vec3 b2, i32 time) {
        initial = begin;
        goal = end;
        bezier_1 = b1;
        bezier_2 = b2;
        end_time = time;
        method_for_3d = InterpMethod::BEZIER;
        this->time = 0;
        flags = 0;
    }

    vec3 step() {
        if (end_time != 0) {
            if (end_time < 0 || ++time < end_time) {
                if (flags == 0) {
                    if (method_for_3d == InterpMethod::CSTSPEED) {
                        initial += goal;
                        current = initial;
                    } else if (method_for_3d == InterpMethod::CSTACCEL) {
                        initial += bezier_2;
                        bezier_2 += goal;
                        current = initial;
                    } else if (method_for_3d == InterpMethod::BEZIER) {
                        f32 x = time.current_f /
                                  static_cast<f32>(end_time);
                        vec3 a0 = initial;
                        vec3 a1 = bezier_1;
                        vec3 a2 = (goal - initial) * 3.f - 
                                       bezier_1 * 2.f - bezier_2;
                        vec3 a3 = (initial - goal) * 2.f
                                     + bezier_1 + bezier_2;
                        current = a0 + a1 * x + a2 * x * x + a3 * x * x * x;
                    } else {
                        current = (goal - initial) *
                            getInterpFunc(method_for_3d)(
                                time.current_f /
                                static_cast<f32>(end_time)) + initial;
                    }
                } else {
                    for (int i = 0; i < 3; i++) {
                        if (method_for_1d[i] == InterpMethod::CSTSPEED) {
                            initial[i] += goal[i];
                            current[i] = initial[i];
                        } else if (method_for_1d[i] == InterpMethod::CSTACCEL) {
                            initial[i] += bezier_2[i];
                            bezier_2[i] += goal[i];
                            current[i] = initial[i];
                        } else if (method_for_1d[i] == InterpMethod::BEZIER) {
                            f32 x = time.current_f /
                                      static_cast<f32>(end_time);
                            f32 a0 = initial[i];
                            f32 a2 = 3.f * (goal[i] - initial[i]);
                            f32 a3 = 2.f * (initial[i] - goal[i]);
                            current[i] = a0 + a2 * x * x + a3 * x * x * x;
                        } else {
                            current[i] = (goal[i] - initial[i]) *
                                getInterpFunc(method_for_1d[i])(
                                time.current_f /
                                static_cast<f32>(end_time)) + initial[i];
                        }
                    }
                }
                return current;
            }
            time = end_time;
            end_time = 0;
        }
        if (method_for_3d != InterpMethod::CSTSPEED && method_for_3d != InterpMethod::CSTACCEL) {
            return goal;
        }
        return initial;
    }
};

} // namespace ns

#endif // INTERP_HEADER_INCLUDED
