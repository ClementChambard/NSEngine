#include "./DefaultShader.hpp"

namespace ns {

const char * const DefaultShader::VERTEX_SHADER_PATH = "assets/engine/shaders/builtin.default.vert.glsl";
const char * const DefaultShader::FRAGMENT_SHADER_PATH = "assets/engine/shaders/builtin.default.frag.glsl";

void DefaultShader::bind_attributes() {
  bind_attribute(0, "VertexPosition");
  bind_attribute(1, "VertexColor");
  bind_attribute(2, "VertexTexCoords");
}

void DefaultShader::get_all_uniform_location() {
  m_location_viewMatrix = get_uniform_location("viewMatrix");
  m_location_projectionMatrix = get_uniform_location("projectionMatrix");
}

void DefaultShader::set_view_matrix(mat4 const& mat) {
  load_mat4(m_location_viewMatrix, mat);
}

void DefaultShader::set_projection_matrix(mat4 const& mat) {
  load_mat4(m_location_projectionMatrix, mat);
}

} // namespace ns
