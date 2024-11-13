#ifndef VEC_HEADER_INCLUDED
#define VEC_HEADER_INCLUDED

#include <defines.h>
#include "./ivec2.hpp"
#include "./vec2.hpp"
#include "./vec3.hpp"
#include "./vec4.hpp"

namespace ns {

using glm::dot;
using glm::cross;
using glm::normalize;

template<typename T> inline f32 length_sq(T v) { return glm::dot(v, v); }
template<typename T> inline f32 length(T v) { return glm::sqrt(length_sq(v)); }

} // namespace ns

#endif // VEC_HEADER_INCLUDED
