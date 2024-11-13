#include "./math.hpp"
#include "../platform/platform.h"

#include <cmath>

static bool g_rand_seeded = false;

namespace ns {

i32 rand() {
    if (!g_rand_seeded) {
        srand(static_cast<u32>(platform::get_absolute_time()));
        g_rand_seeded = true;
    }
    return ::rand();
}

i32 randrange(i32 min, i32 max) { return (ns::rand() % (max - min + 1)) + min; }
f32 frand() { return static_cast<f32>(ns::rand()) / static_cast<f32>(RAND_MAX); }
f32 frandrange(f32 min, f32 max) { return ns::frand() * (max - min) + min; }

f32 barycentric(ns::vec3 p1, ns::vec3 p2, ns::vec3 p3, ns::vec2 pos) {
    f32 det = (p2.z-p3.z)*(p1.x-p3.x)+(p3.x-p2.x)*(p1.z-p3.z);
    f32 l1 = ((p2.z-p3.z)*(pos.x-p3.x)+(p3.x-p2.x)*(pos.y-p3.z))/det;
    f32 l2 = ((p3.z-p1.z)*(pos.x-p3.x)+(p1.x-p3.x)*(pos.y-p3.z))/det;
    f32 l3 = 1-l1-l2;
    return l1*p1.y+l2*p2.y+l3*p3.y;
}

f32 point_distance_to_segment(ns::vec2 s1, ns::vec2 s2, ns::vec2 p) {
    f32 tempt = (p.x-s1.x)*(s2.x-s1.x)+(p.y-s1.y)*(s2.y-s1.y);
    f32 dist = point_distance_sq(s1.x,s1.y,s2.x,s2.y);
    f32 t = ns::clamp(tempt / dist,0.f,1.f);

    return point_distance(p.x,p.y,s1.x+(s2.x-s1.x)*t,s1.y+(s2.y-s1.y)*t);
}

bool segment_intersect(ns::vec2 a1, ns::vec2 a2, ns::vec2 b1, ns::vec2 b2) {
    // Find the four orientations needed for general and
    // special cases
    auto sign = [](f32 var) -> f32 {
        if (var < 0.f) return -1.f;
        if (var > 0.f) return 1.f;
        return var;
    };
    int o1 = sign((b1.y - a1.y) * (a2.x - b1.x) - (b1.x - a1.x) * (a2.y - b1.y));
    int o2 = sign((b2.y - a1.y) * (a2.x - b2.x) - (b2.x - a1.x) * (a2.y - b2.y));
    int o3 = sign((a1.y - b1.y) * (b2.x - a1.x) - (a1.x - b1.x) * (b2.y - a1.y));
    int o4 = sign((a2.y - b1.y) * (b2.x - a2.x) - (a2.x - b1.x) * (b2.y - a2.y));

    // General case
    if (o1 != o2 && o3 != o4)
        return true;

    // Special Cases
    // p1, q1 and p2 are colinear and p2 lies on segment p1q1
    if (o1 == 0 && point_distance_to_segment(a1,a2,b1)==0) return true;

    // p1, q1 and q2 are colinear and q2 lies on segment p1q1
    if (o2 == 0 && point_distance_to_segment(a1,a2,b1)==0) return true;

    // p2, q2 and p1 are colinear and p1 lies on segment p2q2
    if (o3 == 0 && point_distance_to_segment(b1,b2,a1)==0) return true;

        // p2, q2 and q1 are colinear and q1 lies on segment p2q2
    if (o4 == 0 && point_distance_to_segment(b1,b2,a1)==0) return true;

    return false; // Doesn't fall in any of the above cases
}

f32 segment_distance_x(ns::vec2 a1, ns::vec2 a2, ns::vec2 b1, ns::vec2 b2, bool min) {
    f32 temp;
    if (a1.y > a2.y) temp = a1.y, a1.y = a2.y, a2.y = temp;
    if (b1.y > b2.y) temp = b1.y, b1.y = b2.y, b2.y = temp;

    if (a1.y > b2.y || b1.y > a2.y) return 0;

    f32 disttop = 0;
    f32 distbottom = 0;

    if (b1.y < a1.y) disttop = b1.x+(b2.x-b1.x)*(a1.y-b1.y)/(b2.y-b1.y)-a1.x;
    else disttop = b1.x - (a1.x+(a2.x-a1.x)*(b1.y-a1.y)/(a2.y-a1.y));
    if (b2.y < a2.y) distbottom = b2.x - (a1.x+(a2.x-a1.x)*(b2.y-a1.y)/(a2.y-a1.y));
    else distbottom = b1.x+(b2.x-b1.x)*(a2.y-b1.y)/(b2.y-b1.y)-a2.x;

    return min?fmin(disttop,distbottom):fmax(disttop,distbottom);
}

f32 segment_distance_y(ns::vec2 a1, ns::vec2 a2, ns::vec2 b1, ns::vec2 b2, bool min) {
    f32 temp;
    if (a1.x > a2.x) temp = a1.x, a1.x = a2.x, a2.x = temp;
    if (b1.x > b2.x) temp = b1.x, b1.x = b2.x, b2.x = temp;

    if (a1.x > b2.x || b1.x > a2.x) return 0;

    f32 distleft = 0;
    f32 distright = 0;

    if (b1.x < a1.x) distleft = b1.y+(b2.y-b1.y)*(a1.x-b1.x)/(b2.x-b1.x)-a1.y;
    else distleft = b1.y - a1.y+(a2.y-a1.y)*(b1.x-a1.x)/(a2.x-a1.x);
    if (b2.x < a2.x) distright = b2.y - a1.y+(a2.y-a1.y)*(b2.x-a1.x)/(a2.x-a1.x);
    else distright = b1.y+(b2.y-b1.y)*(a2.x-b1.x)/(b2.x-b1.x)-a2.y;

    return min?fmin(distleft,distright):fmax(distleft,distright);
}

f32 point_direction(f32 x1, f32 y1, f32 x2, f32 y2) {
    if (x1 == x2 && y1 == y2) return 0;
    f32 a = atan((f32)(y2-y1) / (f32)(x2 - x1));
    if ((x2-x1) < 0) a -= ns::PI<f32>;
    if (a < -ns::PI<f32>) a+=2*ns::PI<f32>;
    return a;
}

f32 point_distance_sq(f32 x1, f32 y1, f32 x2, f32 y2) { f32 a = (x2-x1), b = (y2-y1); return a*a+b*b; }
f32 point_distance_sq(f32 x1, f32 y1, f32 z1, f32 x2, f32 y2, f32 z2) { f32 a = (x2-x1), b = (y2-y1), c = (z2-z1); return a*a+b*b+c*c; }
f32 point_distance(f32 x1, f32 y1, f32 x2, f32 y2) { return sqrt(point_distance_sq(x1,y1,x2,y2)); }
f32 point_distance(f32 x1, f32 y1, f32 z1, f32 x2, f32 y2, f32 z2) { return sqrt(point_distance_sq(x1,y1,z1,x2,y2,z2)); }

f32 lengthdir_x(f32 length, f32 direction) { return length * cos(direction); }
f32 lengthdir_y(f32 length, f32 direction) { return length * sin(direction); }

ns::vec2 lengthdir_vec(f32 length, f32 direction) { return ns::vec2(lengthdir_x(length, direction), lengthdir_y(length, direction)); }
ns::vec3 lengthdir_vec3(f32 length, f32 direction) { return ns::vec3(lengthdir_x(length, direction), lengthdir_y(length, direction), 0); }

bool point_in_rectangle(f32 x, f32 y, f32 x1, f32 y1, f32 x2, f32 y2) {
    f32 temp;
    if (x2 < x1) temp = x1, x1 = x2, x2 = temp;
    if (y2 < y1) temp = y1, y1 = y2, y2 = temp;
    return (x <= x2 && x >= x1 && y <= y2 && y >= y1);
}

bool rectangle_intersect(f32 x1, f32 y1, f32 x2, f32 y2, f32 x3, f32 y3, f32 x4, f32 y4, bool equals) {
    f32 temp;
    if (x2 < x1) temp = x1, x1 = x2, x2 = temp;
    if (y2 < y1) temp = y1, y1 = y2, y2 = temp;
    if (x4 < x3) temp = x3, x3 = x4, x4 = temp;
    if (y4 < y3) temp = y3, y3 = y4, y4 = temp;
    if (equals) {
        if (x1 >= x4 || x2 <= x3 || y1 >= y4 || y2 <= y3) return false;
        return true;
    } else {
        if (x1 > x4 || x2 < x3 || y1 > y4 || y2 < y3) return false;
        return true;
    }
}

}
