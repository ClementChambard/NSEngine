#ifndef STATICSHADER_INCLUDED_H
#define STATICSHADER_INCLUDED_H

#include "../ShaderProgram.h"
#include "Light.h"
#include <vector>

namespace NS3D {

class StaticShader : public ns::ShaderProgram {
    public:
        StaticShader() : ShaderProgram(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH) { bindAttributes(); getAllUniformLocation(); }
        
        void SetProjectionMatrix(const glm::mat4& matrix);
        void SetViewMatrix(const glm::mat4& matrix);
        void SetModelMatrix(const glm::mat4& matrix);
        void SetLights(const std::vector<const Light*>& lights);
        void SetTextureVars(f32 damper, f32 reflect, bool useFakeNormals);
        void SetSkyColor(const glm::vec3& col);
        void SetNumberOfRows(i32 num);
        void SetOffset(const glm::vec2& off);
        void SetClipPlane(const glm::vec4& pla);

    protected:
        void bindAttributes() override;
        void getAllUniformLocation() override;

    private:
        static cstr VERTEX_SHADER_PATH;
        static cstr FRAGMENT_SHADER_PATH;

        GLuint location_projectionMatrix;
        GLuint location_viewMatrix;
        GLuint location_modelMatrix;
        GLuint location_lightPosition[MAX_LIGHTS];
        GLuint location_lightColor[MAX_LIGHTS];
        GLuint location_attenuation[MAX_LIGHTS];
        GLuint location_shineDamper;
        GLuint location_reflectivity;
        GLuint location_useFakeLighting;
        GLuint location_skyColor;
        GLuint location_numberOfRows;
        GLuint location_offset;
        GLuint location_clipPlane;
};

}

#endif
