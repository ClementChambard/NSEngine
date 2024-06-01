#ifndef SKYBOXSHADER_INCLUDED_H
#define SKYBOXSHADER_INCLUDED_H

#include "../ShaderProgram.h"

namespace NS3D {

class SkyboxShader : public ns::ShaderProgram {
    public:
       SkyboxShader() : ShaderProgram(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH) { bindAttributes(); getAllUniformLocation(); }

       void SetProjectionMatrix(const glm::mat4& mat);
       void SetViewMatrix(const glm::mat4& mat);

    protected:
        void bindAttributes() override;
        void getAllUniformLocation() override;

    private:
        static cstr VERTEX_SHADER_PATH;
        static cstr FRAGMENT_SHADER_PATH;

        GLuint location_projectionMatrix;
        GLuint location_viewMatrix;
};

}

#endif
