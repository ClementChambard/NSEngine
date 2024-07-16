#ifndef SHADERPROGRAM_HEADER_INCLUDED
#define SHADERPROGRAM_HEADER_INCLUDED

#include <GL/glew.h>
#include "./math/types/types.hpp"
#include "./defines.h"

namespace ns {

class ShaderProgram {
    public:
        ShaderProgram(cstr vertex_file, cstr fragment_file);
        virtual ~ShaderProgram();

        void use();

    protected:
        virtual void bind_attributes() {}
        virtual void get_all_uniform_location() {}
        void bind_attribute(GLuint attribute, cstr variable_name);
        GLuint get_uniform_location(cstr uniform_name);
        void load_float(GLuint location, GLfloat value);
        void load_vec2(GLuint location, const vec2& value);
        void load_vec3(GLuint location, const vec3& value);
        void load_vec4(GLuint location, const vec4& value);
        void load_mat4(GLuint location, const mat4& value);
        void load_bool(GLuint location, bool value) { load_float(location, (GLfloat)value); }
        void load_int(GLuint location, GLint value);

    private:
        GLuint m_programID = 0;
        GLuint m_vertexShaderID = 0;
        GLuint m_fragmentShaderID = 0;

        static GLuint load_shader(cstr file, GLenum type);
};

} // namespace ns

#endif // SHADERPROGRAM_HEADER_INCLUDED
