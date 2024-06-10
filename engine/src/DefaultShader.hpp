#ifndef DEFAULTSHADER_INCLUDED_H
#define DEFAULTSHADER_INCLUDED_H

#include "./ShaderProgram.h"

namespace ns {

class DefaultShader : public ShaderProgram {
public:
  DefaultShader() : ShaderProgram(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH) { bindAttributes(); getAllUniformLocation(); }

  void setProjectionMatrix(const glm::mat4& mat);
  void setViewMatrix(const glm::mat4& mat);

protected:
  void bindAttributes();
  void getAllUniformLocation();

private:
  static const char * const VERTEX_SHADER_PATH;
  static const char * const FRAGMENT_SHADER_PATH;

  GLuint location_projectionMatrix;
  GLuint location_viewMatrix;
  
};

} // namespace ns

#endif // DEFAULTSHADER_INCLUDED_H
