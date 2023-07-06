#ifndef MATH_H
#define MATH_H

#include <iostream>
#include <math.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <functional>

#define PI 3.141592653589793f
#define PI2 6.283185307179586f
#define PI1_2 1.5707963267948966f
#define PI1_4 0.7853981633974483f
#define PI1_6 0.5235987755982988f

#define SQRT2 1.4142135623730951f
#define SQRT1_2 0.7071067811865476f

namespace math {

    inline glm::vec3 Vec3Mat4(const glm::vec3& v, const glm::mat4& m)
    {
        glm::vec4 result = m * glm::vec4(v.x, v.y, v.z, 1.f);
        return glm::vec3(result.x, result.y, result.z);
    }

    inline glm::vec3 GetAnyPerpendicularUnitVector(const glm::vec3& vec)
    {
      if (vec.y != 0.0f || vec.z != 0.0f)
        return glm::vec3(1, 0, 0);
      else
        return glm::vec3(0, 1, 0);
    }

    template<typename T>
    extern void swap(T& var1, T& var2);

    template<typename T>
    inline T min(T var1, T var2)
    {
        if (var1<var2) return var1;
        else return var2;
    }
    template<typename T>
    inline T min_vector(std::vector<T> vec)
    {
        if (vec.empty()) return 0;
        else if (vec.size() == 1) return vec[0];
        T min = vec[0];
        for (int i = 1; i < vec.size(); i++) if (vec[i]<min) min = vec[i];
        return min;
    }
    template<typename T>
    inline T max(T var1, T var2)
    {
        if (var1>var2) return var1;
        else return var2;
    }
    template<typename T>
    inline void swap(T& var1, T& var2)
    {
        T temp = var1;
        var1 = var2;
        var2 = temp;
    }
    template<typename T>
    inline T clamp(T var1, T var2, T var3)
    {
        if (var2 > var3) swap(var2,var3);
        if (var1 < var2) return var2;
        if (var1 > var3) return var3;
        return var1;
    }
    inline float vec3maxval(glm::vec3 in)
    {
        return fmax(in.x,fmax(in.y,in.z));
    }
    inline float vecAxisVal(glm::vec3 v, glm::vec3 axis)
    {
        v *= axis;
        if (v.x != 0) return v.x;
        else if (v.y != 0) return v.y;
        else if (v.z != 0) return v.z;
        else return 0;
    }
    inline glm::vec3 vec3max(glm::vec3 in)
    {
        float m = vec3maxval(in);
        if (in.x == m) return in * glm::vec3(1,0,0);
        else if (in.y == m) return in * glm::vec3(0,1,0);
        else return in * glm::vec3(0,0,1);
    }
    inline glm::vec3 vec3max(glm::vec3 in1, glm::vec3 in2)
    {
        return vec3maxval(in1) > vec3maxval(in2) ? in1 : in2;
    }
    inline glm::vec3 maxvec3(glm::vec3 in1, glm::vec3 in2)
    {
        return {fmax(in1.x,in2.x),fmax(in1.y,in2.y),fmax(in1.z,in2.z)};
    }
    inline glm::mat4 makeTransformationMatrix(const glm::vec3& translation, const glm::vec3& rotation, const glm::vec3& scale)
    {
        glm::mat4 matrix = glm::mat4(1);
        matrix = glm::translate(matrix, translation);
        matrix = glm::rotate(matrix, rotation.x, {1,0,0});
        matrix = glm::rotate(matrix, rotation.y, {0,1,0});
        matrix = glm::rotate(matrix, rotation.z, {0,0,1});
        matrix = glm::scale(matrix, scale);
        return matrix;
    }

    template<typename T>
    inline T sign(T var)
    {
        if (var < T(0)) return T(-1);
        if (var > T(0)) return T(1);
        return var;
    }
    inline float mod(float var1, float var2)
    {
        float a = var1/var2;
        float b = a-(float)floor(a);
        float r = var2*b;
        return r;
    }

    inline float barycentric(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec2 pos)
    {
        float det = (p2.z-p3.z)*(p1.x-p3.x)+(p3.x-p2.x)*(p1.z-p3.z);
        float l1 = ((p2.z-p3.z)*(pos.x-p3.x)+(p3.x-p2.x)*(pos.y-p3.z))/det;
        float l2 = ((p3.z-p1.z)*(pos.x-p3.x)+(p1.x-p3.x)*(pos.y-p3.z))/det;
        float l3 = 1-l1-l2;
        return l1*p1.y+l2*p2.y+l3*p3.y;
    }

    inline glm::vec2 clamp(glm::vec2 v, glm::vec2 min, glm::vec2 max)
    {
        v.x = clamp(v.x, min.x, max.x);
        v.y = clamp(v.y, min.y, max.y);
        return v;
    }
    template<typename T>
    inline T wrap(T val, T min, T max)
    {
        int i = 32;
        while(val >= max && i--) val -= (max-min);
        while(val < min && i--) val += (max-min);
        return val;
    }
    inline void angle_normalize(float& a) { a = wrap(a, -PI, PI); }

    extern float point_distance_to_segment(glm::vec2 s1, glm::vec2 s2, glm::vec2 p);
    extern bool segment_intersect(glm::vec2 a1, glm::vec2 a2, glm::vec2 b1, glm::vec2 b2);
    extern float segment_distance_x(glm::vec2 a1, glm::vec2 a2, glm::vec2 b1, glm::vec2 b2,bool min);
    extern float segment_distance_y(glm::vec2 a1, glm::vec2 a2, glm::vec2 b1, glm::vec2 b2,bool min);

    extern float point_distance_sq(float x1, float y1, float x2, float y2);
    extern float point_distance_sq(float x1, float y1, float z1, float x2, float y2, float z2);
    extern float point_distance(float x1, float y1, float x2, float y2);
    extern float point_distance(float x1, float y1, float z1, float x2, float y2, float z2);
    extern float point_direction(float x1, float y1, float x2, float y2);

    inline float point_distance_sq(glm::vec2 const& p1, glm::vec2 const& p2) { return point_distance_sq(p1.x, p1.y, p2.x, p2.y); }
    inline float point_distance(glm::vec2 const& p1, glm::vec2 const& p2) { return point_distance(p1.x, p1.y, p2.x, p2.y); }
    inline float point_distance_sq(glm::vec3 const& p1, glm::vec3 const& p2) { return point_distance_sq(p1.x, p1.y, p1.z, p2.x, p2.y, p2.z); }
    inline float point_distance(glm::vec3 const& p1, glm::vec3 const& p2) { return point_distance(p1.x, p1.y, p1.z, p2.x, p2.y, p2.z); }
    inline float point_direction(glm::vec2 const& p1, glm::vec2 const& p2) { return point_direction(p1.x, p1.y, p2.x, p2.y); }

    extern float lengthdir_x(float length, float direction);
    extern float lengthdir_y(float length, float direction);
    extern glm::vec2 lengthdir_vec(float length, float direction);

    extern bool point_in_rectangle(float x, float y, float x1, float y1, float x2, float y2);
    extern bool rectangle_intersect(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, bool equal = false);
    inline bool rectangle_intersect(glm::vec4 r1, glm::vec4 r2, bool equal = false)
    {
        return rectangle_intersect(r1.x,r1.y,r1.z,r1.w,r2.x,r2.y,r2.z,r2.w,equal);
    }

    //template <typename T>
    inline void shift_vector(std::vector<float>& v, float firstvalue)
    {
        for (size_t i = v.size()-1; i > 0; i--)
        {
            v[i] = v[i-1];
        }
        v[0] = firstvalue;
    }

    extern float warp(float val, float min, float max);
}
    
#endif
