#ifndef MATH_HEADER_INCLUDED
#define MATH_HEADER_INCLUDED

#include "../defines.h"
#include "./cstes.hpp"
#include "./types/types.hpp"

namespace ns {

NS_API f32 sin(f32 x);
NS_API f32 cos(f32 x);
NS_API f32 tan(f32 x);
NS_API f32 acos(f32 x);
NS_API f32 sqrt(f32 x);
NS_API f32 abs(f32 x);
NS_API f32 mod(f32 x, f32 y);
NS_API i32 floor(f32 x);

NS_INLINE bool is_power_of_2(u64 value) {
    return (value != 0) && ((value & (value - 1)) == 0);
}

NS_API i32 rand();
NS_API i32 randrange(i32 min, i32 max);
NS_API f32 frand();
NS_API f32 frandrange(f32 min, f32 max);

NS_INLINE f32 deg_2_rad(f32 degrees) { return degrees * DEG_2_RAD<f32>; }
NS_INLINE f32 rad_2_deg(f32 radians) { return radians * RAD_2_DEG<f32>; }

} // namespace ns

namespace Random {

inline f32 Float01() { return ns::frand(); }
inline f32 Floatm11() { return ns::frandrange(-1.0f, 1.0f); }
inline f32 Angle() { return ns::frandrange(-ns::PI<f32>, ns::PI<f32>); }

} // namespcae Random

namespace math {

    template<typename T>
    inline T min(T var1, T var2)
    {
        if (var1<var2) return var1;
        else return var2;
    }
    template<typename T>
    inline T max(T var1, T var2)
    {
        if (var1>var2) return var1;
        else return var2;
    }
    template<typename T>
    inline T clamp(T var1, T var2, T var3)
    {
        if (var2 > var3) {
            T temp = var1;
            var1 = var2;
            var2 = temp;
        }
        if (var1 < var2) return var2;
        if (var1 > var3) return var3;
        return var1;
    }

    inline ns::vec2 clamp(ns::vec2 v, ns::vec2 min, ns::vec2 max)
    {
        v.x = clamp(v.x, min.x, max.x);
        v.y = clamp(v.y, min.y, max.y);
        return v;
    }

    inline void angle_normalize(f32& a) { 
        int i = 32;
        while(a >= ns::PI<f32> && i--) a -= ns::PI_2<f32>;
        while(a < -ns::PI<f32> && i--) a += ns::PI_2<f32>;
    }

    extern f32 point_distance_sq(f32 x1, f32 y1, f32 x2, f32 y2);
    extern f32 point_distance_sq(f32 x1, f32 y1, f32 z1, f32 x2, f32 y2, f32 z2);
    extern f32 point_distance(f32 x1, f32 y1, f32 x2, f32 y2);
    extern f32 point_distance(f32 x1, f32 y1, f32 z1, f32 x2, f32 y2, f32 z2);
    extern f32 point_direction(f32 x1, f32 y1, f32 x2, f32 y2);

    inline f32 point_distance_sq(ns::vec2 const& p1, ns::vec2 const& p2) { return point_distance_sq(p1.x, p1.y, p2.x, p2.y); }
    inline f32 point_distance(ns::vec2 const& p1, ns::vec2 const& p2) { return point_distance(p1.x, p1.y, p2.x, p2.y); }
    inline f32 point_distance_sq(ns::vec3 const& p1, ns::vec3 const& p2) { return point_distance_sq(p1.x, p1.y, p1.z, p2.x, p2.y, p2.z); }
    inline f32 point_distance(ns::vec3 const& p1, ns::vec3 const& p2) { return point_distance(p1.x, p1.y, p1.z, p2.x, p2.y, p2.z); }
    inline f32 point_direction(ns::vec2 const& p1, ns::vec2 const& p2) { return point_direction(p1.x, p1.y, p2.x, p2.y); }

    inline f32 veclensq(ns::vec2 const& v) { return point_distance_sq({0,0}, v); }
    inline f32 veclen(ns::vec2 const& v) { return point_distance({0,0}, v); }
    inline f32 veclensq(ns::vec3 const& v) { return point_distance_sq({0,0,0}, v); }
    inline f32 veclen(ns::vec3 const& v) { return point_distance({0,0,0}, v); }

    extern f32 lengthdir_x(f32 length, f32 direction);
    extern f32 lengthdir_y(f32 length, f32 direction);
    extern ns::vec2 lengthdir_vec(f32 length, f32 direction);
    extern ns::vec3 lengthdir_vec3(f32 length, f32 direction);

    extern f32 point_distance_to_segment(ns::vec2 s1, ns::vec2 s2, ns::vec2 p);
    extern bool segment_intersect(ns::vec2 a1, ns::vec2 a2, ns::vec2 b1, ns::vec2 b2);
    extern f32 segment_distance_x(ns::vec2 a1, ns::vec2 a2, ns::vec2 b1, ns::vec2 b2,bool min);
    extern f32 segment_distance_y(ns::vec2 a1, ns::vec2 a2, ns::vec2 b1, ns::vec2 b2,bool min);
    inline f32 barycentric(ns::vec3 p1, ns::vec3 p2, ns::vec3 p3, ns::vec2 pos)
    {
        f32 det = (p2.z-p3.z)*(p1.x-p3.x)+(p3.x-p2.x)*(p1.z-p3.z);
        f32 l1 = ((p2.z-p3.z)*(pos.x-p3.x)+(p3.x-p2.x)*(pos.y-p3.z))/det;
        f32 l2 = ((p3.z-p1.z)*(pos.x-p3.x)+(p1.x-p3.x)*(pos.y-p3.z))/det;
        f32 l3 = 1-l1-l2;
        return l1*p1.y+l2*p2.y+l3*p3.y;
    }

    extern bool point_in_rectangle(f32 x, f32 y, f32 x1, f32 y1, f32 x2, f32 y2);
    extern bool rectangle_intersect(f32 x1, f32 y1, f32 x2, f32 y2, f32 x3, f32 y3, f32 x4, f32 y4, bool equal = false);
    inline bool rectangle_intersect(ns::vec4 r1, ns::vec4 r2, bool equal = false)
    {
        return rectangle_intersect(r1.x,r1.y,r1.z,r1.w,r2.x,r2.y,r2.z,r2.w,equal);
    }
}
    
#endif // MATH_HEADER_INCLUDED
