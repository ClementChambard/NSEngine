#include "ShaderProgram.h"
#include "FileOpener.h"
#include "Error.h"

namespace ns {

    ShaderProgram::ShaderProgram(cstr vertexFile, cstr fragmentFile)
    {
        vertexShaderID = loadShader(vertexFile, GL_VERTEX_SHADER);
        fragmentShaderID = loadShader(fragmentFile, GL_FRAGMENT_SHADER);
        programID = glCreateProgram();
        glAttachShader(programID, vertexShaderID);
        glAttachShader(programID, fragmentShaderID);
        bindAttributes();
        glLinkProgram(programID);
        glValidateProgram(programID);
        getAllUniformLocation();
    }

    ShaderProgram::~ShaderProgram()
    {
        stop();
        glDetachShader(programID, vertexShaderID);
        glDetachShader(programID, fragmentShaderID);
        glDeleteShader(vertexShaderID);
        glDeleteShader(fragmentShaderID);
        glDeleteProgram(programID);
    }

    void ShaderProgram::bindAttribute(GLuint attribute, cstr attributeName)
    {
        glBindAttribLocation(programID, attribute, attributeName);
    }

    GLuint ShaderProgram::getUniformLocation(cstr uniformName)
    {
        return glGetUniformLocation(programID, uniformName);
    }

    void ShaderProgram::loadFloat(GLuint location, GLfloat value) { glUniform1f(location, value); }
    void ShaderProgram::loadVec2(GLuint location, const glm::vec2& value) { glUniform2f(location, value.x, value.y); }
    void ShaderProgram::loadVec3(GLuint location, const glm::vec3& value) { glUniform3f(location, value.x, value.y, value.z); }
    void ShaderProgram::loadVec4(GLuint location, const glm::vec4& value) { glUniform4f(location, value.x, value.y, value.z, value.w); }
    void ShaderProgram::loadMat4(GLuint location, const glm::mat4& value) { glUniformMatrix4fv(location, 1, GL_FALSE, &(value[0][0])); }
    void ShaderProgram::loadInt(GLuint location, GLint value) { glUniform1i(location, value); }

    GLuint ShaderProgram::loadShader(cstr file, GLenum type)
    {
        std::string shaderSource;
        FileOpener::readFileToBuffer(file, shaderSource);
        GLuint id = glCreateShader(type);
        const char* shaderSourceCstr = shaderSource.c_str();
        glShaderSource(id, 1, &shaderSourceCstr, nullptr);
        glCompileShader(id);
        GLint success = 0;
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if (success == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);
            std::vector<char> errorLog(maxLength);
            glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);
            glDeleteShader(id); //Don't leak the shader.
            std::printf("%s\n", &(errorLog[0]));
            fatalError("Shader failed to compile");
        }
        return id;
    }

}
