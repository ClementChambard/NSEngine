#ifndef MATH_HEADER_INCLUDED
#define MATH_HEADER_INCLUDED

#include "../defines.h"
#include "./cstes.hpp"
#include "./types/types.hpp"

#include <glm/glm.hpp>
#include <glm/trigonometric.hpp>

namespace ns {

using glm::sin;
using glm::cos;
using glm::tan;
using glm::acos;
using glm::atan;
using glm::sqrt;
using glm::abs;
using glm::mod;
using glm::floor;
using glm::min;
using glm::max;
using glm::clamp;

NS_INLINE bool is_power_of_2(u64 value) {
    return (value != 0) && ((value & (value - 1)) == 0);
}

NS_INLINE void angle_normalize(f32& a) {
    int i = 32;
    while(a >= ns::PI<f32> && i--) a -= ns::PI_2<f32>;
    while(a < -ns::PI<f32> && i--) a += ns::PI_2<f32>;
}

NS_API i32 rand();
NS_API i32 randrange(i32 min, i32 max);
NS_API f32 frand();
NS_API f32 frandrange(f32 min, f32 max);

NS_INLINE f32 deg_2_rad(f32 degrees) { return degrees * DEG_2_RAD<f32>; }
NS_INLINE f32 rad_2_deg(f32 radians) { return radians * RAD_2_DEG<f32>; }

NS_INLINE f32 frandm11() { return frandrange(-1.0f, 1.0f); }
NS_INLINE f32 frandangle() { return frandrange(-PI<f32>, PI<f32>); }

f32 point_distance_sq(f32 x1, f32 y1, f32 x2, f32 y2);
f32 point_distance_sq(f32 x1, f32 y1, f32 z1, f32 x2, f32 y2, f32 z2);
f32 point_distance(f32 x1, f32 y1, f32 x2, f32 y2);
f32 point_distance(f32 x1, f32 y1, f32 z1, f32 x2, f32 y2, f32 z2);
f32 point_direction(f32 x1, f32 y1, f32 x2, f32 y2);

NS_INLINE f32 point_distance_sq(ns::vec2 const& p1, ns::vec2 const& p2) { return point_distance_sq(p1.x, p1.y, p2.x, p2.y); }
NS_INLINE f32 point_distance(ns::vec2 const& p1, ns::vec2 const& p2) { return point_distance(p1.x, p1.y, p2.x, p2.y); }
NS_INLINE f32 point_distance_sq(ns::vec3 const& p1, ns::vec3 const& p2) { return point_distance_sq(p1.x, p1.y, p1.z, p2.x, p2.y, p2.z); }
NS_INLINE f32 point_distance(ns::vec3 const& p1, ns::vec3 const& p2) { return point_distance(p1.x, p1.y, p1.z, p2.x, p2.y, p2.z); }
NS_INLINE f32 point_direction(ns::vec2 const& p1, ns::vec2 const& p2) { return point_direction(p1.x, p1.y, p2.x, p2.y); }

f32 lengthdir_x(f32 length, f32 direction);
f32 lengthdir_y(f32 length, f32 direction);
ns::vec2 lengthdir_vec(f32 length, f32 direction);
ns::vec3 lengthdir_vec3(f32 length, f32 direction);

f32 point_distance_to_segment(ns::vec2 s1, ns::vec2 s2, ns::vec2 p);
bool segment_intersect(ns::vec2 a1, ns::vec2 a2, ns::vec2 b1, ns::vec2 b2);
f32 segment_distance_x(ns::vec2 a1, ns::vec2 a2, ns::vec2 b1, ns::vec2 b2,bool min);
f32 segment_distance_y(ns::vec2 a1, ns::vec2 a2, ns::vec2 b1, ns::vec2 b2,bool min);
f32 barycentric(ns::vec3 p1, ns::vec3 p2, ns::vec3 p3, ns::vec2 pos);
bool point_in_rectangle(f32 x, f32 y, f32 x1, f32 y1, f32 x2, f32 y2);
bool rectangle_intersect(f32 x1, f32 y1, f32 x2, f32 y2, f32 x3, f32 y3, f32 x4, f32 y4, bool equal = false);

NS_INLINE bool rectangle_intersect(ns::vec4 r1, ns::vec4 r2, bool equal = false) { return rectangle_intersect(r1.x,r1.y,r1.z,r1.w,r2.x,r2.y,r2.z,r2.w,equal); }

} // namespace ns
    
#endif // MATH_HEADER_INCLUDED
