#ifndef MAT4_HEADER_INCLUDED
#define MAT4_HEADER_INCLUDED

#include <defines.h>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace ns {

using glm::mat4;
using glm::vec3;

namespace mat {

using glm::transpose;
using glm::inverse;
using glm::translate;
using glm::rotate;
using glm::scale;

inline mat4 orthographic(f32 left, f32 right, f32 bottom, f32 top, f32 near_clip, f32 far_clip) { return glm::ortho(left, right, bottom, top, near_clip, far_clip); }
inline mat4 perspective(f32 fov_rad, f32 aspect_ratio, f32 near_clip, f32 far_clip) { return glm::perspective(fov_rad, aspect_ratio, near_clip, far_clip); }
inline mat4 perspective_lh(f32 fov_rad, f32 aspect_ratio, f32 near_clip, f32 far_clip) { return glm::perspectiveLH(fov_rad, aspect_ratio, near_clip, far_clip); }
inline mat4 lookat(vec3 const &position, vec3 const &target, vec3 const &up) { return glm::lookAt(position, target, up); }
inline mat4 lookat_lh(vec3 const &position, vec3 const &target, vec3 const &up) { return glm::lookAtLH(position, target, up); }

inline mat4 mk4_identity() { return glm::mat4(1.0); }
inline mat4 mk4_rotate(f32 r, vec3 axis) { return glm::rotate(glm::mat4(1.0), r, axis); }
inline mat4 mk4_rotate_x(f32 x) { return glm::rotate(glm::mat4(1.0), x, {1, 0, 0}); }
inline mat4 mk4_rotate_y(f32 y) { return glm::rotate(glm::mat4(1.0), y, {0, 1, 0}); }
inline mat4 mk4_rotate_z(f32 z) { return glm::rotate(glm::mat4(1.0), z, {0, 0, 1}); }
inline mat4 mk4_scale(ns::vec3 const& scale) { return glm::scale(glm::mat4(1.0), scale); }
inline mat4 mk4_translate(ns::vec3 const& pos) { return glm::translate(glm::mat4(1.0), pos); }
// TODO: mk_euler ?

inline vec3 forward(ns::mat4 const& m) { return glm::normalize(glm::vec3{-m[2][0], -m[2][1], -m[2][2]}); }
inline vec3 backward(ns::mat4 const& m) { return glm::normalize(glm::vec3{ m[2][0], m[2][1], m[2][2]}); }
inline vec3 up(ns::mat4 const& m) { return glm::normalize(glm::vec3{ m[1][0], m[1][1], m[1][2]}); }
inline vec3 down(ns::mat4 const& m) { return glm::normalize(glm::vec3{ -m[1][0], -m[1][1], -m[1][2]}); }
inline vec3 left(ns::mat4 const& m) { return glm::normalize(glm::vec3{ -m[0][0], -m[0][1], -m[0][2]}); }
inline vec3 right(ns::mat4 const& m) { return glm::normalize(glm::vec3{ m[0][0], m[0][1], m[0][2]}); }

}

} // namespace ns

#endif // MAT4_HEADER_INCLUDED
