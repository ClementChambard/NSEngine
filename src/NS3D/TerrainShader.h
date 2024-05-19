#ifndef TERRAINSHADER_INCLUDED_H
#define TERRAINSHADER_INCLUDED_H

#include "../ShaderProgram.h"
#include "Light.h"
#include <vector>

namespace NS3D {

class TerrainShader : public ns::ShaderProgram {
    public:
        TerrainShader() : ShaderProgram(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH) { bindAttributes(); getAllUniformLocation(); }
        
        void SetProjectionMatrix(const glm::mat4& matrix);
        void SetViewMatrix(const glm::mat4& matrix);
        void SetModelMatrix(const glm::mat4& matrix);
        void SetLights(const std::vector<const Light*>& lights);
        void SetShineVars(f32 damper, f32 reflect);
        void SetSkyColor(const glm::vec3& col);
        void connectTextureUnits();
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
        GLuint location_skyColor;
        GLuint location_backgroundTexture;
        GLuint location_rTexture;
        GLuint location_gTexture;
        GLuint location_bTexture;
        GLuint location_blendMap;
        GLuint location_clipPlane;
};

}

#endif
