#ifndef SHADERPROGRAM_INCLUDED_H
#define SHADERPROGRAM_INCLUDED_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "./defines.h"

namespace ns {

class ShaderProgram {
    public:
        ShaderProgram(cstr vertexFile, cstr fragmentFile);
        virtual ~ShaderProgram();

        void start() { glUseProgram(programID); }
        void stop()  { glUseProgram(0);         }

    protected:
        virtual void bindAttributes() {}
        virtual void getAllUniformLocation() {}
        void bindAttribute(GLuint attribute, cstr variableName);
        GLuint getUniformLocation(cstr uniformName);
        void loadFloat(GLuint location, GLfloat value);
        void loadVec2(GLuint location, const glm::vec2& value);
        void loadVec3(GLuint location, const glm::vec3& value);
        void loadVec4(GLuint location, const glm::vec4& value);
        void loadMat4(GLuint location, const glm::mat4& value);
        void loadBool(GLuint location, bool value) { loadFloat(location, (GLfloat)value); }
        void loadInt(GLuint location, GLint value);

    private:
        GLuint programID = 0;
        GLuint vertexShaderID = 0;
        GLuint fragmentShaderID = 0;

        static GLuint loadShader(cstr file, GLenum type);
};

}

#endif
