#ifndef ANIMSHADER_INCLUDED_H
#define ANIMSHADER_INCLUDED_H

#include "ShaderProgram.h"

namespace ns {

class AnimShader : public ShaderProgram {
    public:
        AnimShader() : ShaderProgram(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH) { bindAttributes(); getAllUniformLocation(); };

        void SetProjectionMatrix(const glm::mat4& mat);
        void SetViewMatrix(const glm::mat4& mat);
        void SetCameraPosition(const glm::vec3& vec);
        void SetFog(f32 min, f32 max, const glm::vec4& color);

    protected:
        void bindAttributes() override;
        void getAllUniformLocation() override;

    private:
        static cstr VERTEX_SHADER_PATH;
        static cstr FRAGMENT_SHADER_PATH;

        GLuint location_projectionMatrix = 0;
        GLuint location_viewMatrix = 0;
        GLuint location_cameraPosition = 0;
        GLuint location_fogColor = 0;
        GLuint location_fogMin = 0;
        GLuint location_fogMax = 0;

};

}

#endif
