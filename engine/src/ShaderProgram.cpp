#include "ShaderProgram.h"
#include "FileOpener.h"
#include "logger.h"

namespace ns {

    static ShaderProgram* g_currentProgram = nullptr;

    ShaderProgram::ShaderProgram(cstr vertexFile, cstr fragmentFile)
    {
        m_vertexShaderID = load_shader(vertexFile, GL_VERTEX_SHADER);
        m_fragmentShaderID = load_shader(fragmentFile, GL_FRAGMENT_SHADER);
        m_programID = glCreateProgram();
        glAttachShader(m_programID, m_vertexShaderID);
        glAttachShader(m_programID, m_fragmentShaderID);
        bind_attributes();
        glLinkProgram(m_programID);
        glValidateProgram(m_programID);
        get_all_uniform_location();
    }

    ShaderProgram::~ShaderProgram()
    {
        if (g_currentProgram == this) {
            glUseProgram(0);
            g_currentProgram = nullptr;
        }
        glDetachShader(m_programID, m_vertexShaderID);
        glDetachShader(m_programID, m_fragmentShaderID);
        glDeleteShader(m_vertexShaderID);
        glDeleteShader(m_fragmentShaderID);
        glDeleteProgram(m_programID);
    }

    void ShaderProgram::bind_attribute(GLuint attribute, cstr attributeName)
    {
        glBindAttribLocation(m_programID, attribute, attributeName);
    }

    GLuint ShaderProgram::get_uniform_location(cstr uniformName)
    {
        return glGetUniformLocation(m_programID, uniformName);
    }

    void ShaderProgram::use() { 
        if (g_currentProgram == this) return;
        glUseProgram(m_programID);
        g_currentProgram = this;
    }

    void ShaderProgram::load_float(GLuint location, GLfloat value) { glUniform1f(location, value); }
    void ShaderProgram::load_vec2(GLuint location, const vec2& value) { glUniform2f(location, value.x, value.y); }
    void ShaderProgram::load_vec3(GLuint location, const vec3& value) { glUniform3f(location, value.x, value.y, value.z); }
    void ShaderProgram::load_vec4(GLuint location, const vec4& value) { glUniform4f(location, value.x, value.y, value.z, value.w); }
    void ShaderProgram::load_mat4(GLuint location, const mat4& value) { glUniformMatrix4fv(location, 1, GL_FALSE, &(value[0][0])); }
    void ShaderProgram::load_int(GLuint location, GLint value) { glUniform1i(location, value); }

    GLuint ShaderProgram::load_shader(cstr file, GLenum type)
    {
        std::string shaderSource;
        FileOpener::read_file_to_buffer(file, shaderSource);
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
            NS_FATAL("Shader '%s' failed to compile: %s", file, &(errorLog[0]));
            exit(1);
        }
        return id;
    }

}
