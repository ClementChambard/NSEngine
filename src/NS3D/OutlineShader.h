#ifndef OUTLINESHADER_H_
#define OUTLINESHADER_H_

#include "../ShaderProgram.h"

namespace NS3D {

    class OutlineShader : public ns::ShaderProgram {
        public:
            OutlineShader() : ShaderProgram(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH) { bindAttributes(); getAllUniformLocation(); }

            void SetProjectionMatrix(const glm::mat4& matrix);
            void SetViewMatrix(const glm::mat4& matrix);
            void SetModelMatrix(const glm::mat4& matrix);
            void SetSkyColor(const glm::vec3& col);
            void SetColor(const glm::vec3& col);

        protected:
            void bindAttributes() override;
            void getAllUniformLocation() override;

        private:
            static cstr VERTEX_SHADER_PATH;
            static cstr FRAGMENT_SHADER_PATH;

            GLuint location_projectionMatrix;
            GLuint location_viewMatrix;
            GLuint location_modelMatrix;
            GLuint location_skyColor;
            GLuint location_color;
    };

}


#endif // OUTLINESHADER_H_
