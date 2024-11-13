#ifndef VEC4_HEADER_INCLUDED
#define VEC4_HEADER_INCLUDED

#include <glm/glm.hpp>

namespace ns {

using glm::vec4;

namespace vec {

inline vec4 one4() { return {1.0f, 1.0f, 1.0f, 1.0f}; }
inline vec4 zero4() { return {0.0f, 0.0f, 0.0f, 0.0f}; }

}

} // namespace ns

#endif // NS_VEC4_HEADER_INCLUDED
