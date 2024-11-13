#ifndef VEC2_HEADER_INCLUDED
#define VEC2_HEADER_INCLUDED

#include <glm/glm.hpp>

namespace ns {

using glm::vec2;

namespace vec {

inline vec2 zero2() { return {0.0f, 0.0f}; }
inline vec2 one2() { return {1.0f, 1.0f}; }
inline vec2 up2() { return {0.0f, 1.0f}; }
inline vec2 down2() { return {0.0f, -1.0f}; }
inline vec2 left2() { return {-1.0f, 0.0f}; }
inline vec2 right2() { return {1.0f, 0.0f}; }

}

} // namespace ns

#endif // VEC2_HEADER_INCLUDED
