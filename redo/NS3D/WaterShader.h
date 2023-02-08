#ifndef WATERSHADER_INCLUDED_H
#define WATERSHADER_INCLUDED_H

#include "../ShaderProgram.h"
#include "Light.h"

namespace NS3D {

    class WaterShader : public NSEngine::ShaderProgram {
        public:
            WaterShader() : ShaderProgram(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH) { bindAttributes(); getAllUniformLocation(); }

            void SetProjectionMatrix(const glm::mat4& mat);
            void SetViewMatrix(const glm::mat4& mat);
            void SetModelMatrix(const glm::mat4& mat);
            void SetCameraPosition(const glm::vec3& pos);
            void SetMoveFactor(const glm::vec2& mv);
            void SetSun(const Light* sun);

            void connectTextures();

        protected:
            void bindAttributes() override;
            void getAllUniformLocation() override;

        private:
            static const std::string VERTEX_SHADER_PATH;
            static const std::string FRAGMENT_SHADER_PATH;

            GLuint location_projectionMatrix;
            GLuint location_viewMatrix;
            GLuint location_modelMatrix;
            GLuint location_cameraPosition;
            GLuint location_reflectionTexture;
            GLuint location_refractionTexture;
            GLuint location_dudvMap;
            GLuint location_normalMap;
            GLuint location_depthMap;
            GLuint location_moveFactor;
            GLuint location_lightPosition;
            GLuint location_lightColor;
    };

}

#endif
