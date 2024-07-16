#ifndef DEFAULTSHADER_HEADER_INCLUDED
#define DEFAULTSHADER_HEADER_INCLUDED

#include "./ShaderProgram.h"

namespace ns {

class DefaultShader : public ShaderProgram {
public:
  DefaultShader() : ShaderProgram(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH) { bind_attributes(); get_all_uniform_location(); }

  void set_projection_matrix(const mat4& mat);
  void set_view_matrix(const mat4& mat);

protected:
  void bind_attributes();
  void get_all_uniform_location();

private:
  static const char * const VERTEX_SHADER_PATH;
  static const char * const FRAGMENT_SHADER_PATH;

  GLuint m_location_projectionMatrix;
  GLuint m_location_viewMatrix;
  
};

} // namespace ns

#endif // DEFAULTSHADER_HEADER_INCLUDED
