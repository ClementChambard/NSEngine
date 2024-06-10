#include "./DefaultShader.hpp"

namespace ns {

const char * const DefaultShader::VERTEX_SHADER_PATH = "assets/engine/shaders/builtin.default.vert.glsl";
const char * const DefaultShader::FRAGMENT_SHADER_PATH = "assets/engine/shaders/builtin.default.frag.glsl";

void DefaultShader::bindAttributes() {
  bindAttribute(0, "VertexPosition");
  bindAttribute(1, "VertexColor");
  bindAttribute(2, "VertexTexCoords");
}

void DefaultShader::getAllUniformLocation() {
  location_viewMatrix = getUniformLocation("viewMatrix");
  location_projectionMatrix = getUniformLocation("projectionMatrix");
}

void DefaultShader::setViewMatrix(glm::mat4 const& mat) {
  loadMat4(location_viewMatrix, mat);
}

void DefaultShader::setProjectionMatrix(glm::mat4 const& mat) {
  loadMat4(location_projectionMatrix, mat);
}

} // namespace ns
