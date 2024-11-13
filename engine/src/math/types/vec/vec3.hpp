#ifndef VEC3_HEADER_INCLUDED
#define VEC3_HEADER_INCLUDED

#include <glm/glm.hpp>

namespace ns {

using glm::vec3;

namespace vec {

inline vec3 zero3() { return {0.0f, 0.0f, 0.0f}; }
inline vec3 one3() { return {1.0f, 1.0f, 1.0f}; }
inline vec3 up3() { return {0.0f, 1.0f, 0.0f}; }
inline vec3 down3() { return {0.0f, -1.0f, 0.0f}; }
inline vec3 left3() { return {-1.0f, 0.0f, 0.0f}; }
inline vec3 right3() { return {1.0f, 0.0f, 0.0f}; }
inline vec3 back3() { return {0.0f, 0.0f, -1.0f}; }
inline vec3 forward3() { return {0.0f, 0.0f, 1.0f}; }

}

} // namespace ns

#endif // VEC3_HEADER_INCLUDED
