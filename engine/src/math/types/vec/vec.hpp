#ifndef VEC_HEADER_INCLUDED
#define VEC_HEADER_INCLUDED

#include "./ivec2.hpp"
#include "./vec2.hpp"
#include "./vec3.hpp"
#include "./vec4.hpp"

namespace ns {

inline vec2::vec2(vec3 const &other) : x(other.x), y(other.y) {}
inline vec2::vec2(vec3 &&other) : x(other.x), y(other.y) {}
inline vec2::vec2(vec4 const &other) : x(other.x), y(other.y) {}
inline vec2::vec2(vec4 &&other) : x(other.x), y(other.y) {}
inline vec3::vec3(vec2 const &other) : x(other.x), y(other.y), z(0.0f) {}
inline vec3::vec3(vec2 &&other) : x(other.x), y(other.y), z(0.0f) {}
inline vec3::vec3(vec4 const &other) : x(other.x), y(other.y), z(other.z) {}
inline vec3::vec3(vec4 &&other) : x(other.x), y(other.y), z(other.z) {}
inline vec4::vec4(vec2 const &other)
    : x(other.x), y(other.y), z(0.0f), w(0.0f) {}
inline vec4::vec4(vec2 &&other) : x(other.x), y(other.y), z(0.0f), w(0.0f) {}
inline vec4::vec4(vec3 const &other)
    : x(other.x), y(other.y), z(other.z), w(0.0f) {}
inline vec4::vec4(vec3 &&other) : x(other.x), y(other.y), z(other.z), w(0.0f) {}
inline vec3& vec3::operator=(vec4 const& other) { x = other.x, y = other.y, z = other.z; return *this; }
inline vec2& vec2::operator=(vec3 const& other) { x = other.x, y = other.y; return *this; }
inline vec2& vec2::operator=(vec4 const& other) { x = other.x, y = other.y; return *this; }

NS_INLINE vec2 operator*(f32 f, vec2 const &v) { return {v.x * f, v.y * f}; }
NS_INLINE vec3 operator*(f32 f, vec3 const &v) {
  return {v.x * f, v.y * f, v.z * f};
}
NS_INLINE vec4 operator*(f32 f, vec4 const &v) {
  return {v.x * f, v.y * f, v.z * f, v.w * f};
}

} // namespace ns

#endif // VEC_HEADER_INCLUDED
