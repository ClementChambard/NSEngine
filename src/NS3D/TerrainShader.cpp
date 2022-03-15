#include "TerrainShader.h"

namespace NS3D {

    const std::string TerrainShader::VERTEX_SHADER_PATH = "assets/engine/shaders/terrainVertexShader";
    const std::string TerrainShader::FRAGMENT_SHADER_PATH = "assets/engine/shaders/terrainFragmentShader";

    void TerrainShader::bindAttributes()
    {
        bindAttribute(0, "position");
        bindAttribute(1, "texCoords");
        bindAttribute(2, "normal");
    }

    void TerrainShader::getAllUniformLocation()
    {
        location_projectionMatrix  = getUniformLocation("projectionMatrix" );
        location_viewMatrix        = getUniformLocation("viewMatrix"       );
        location_modelMatrix       = getUniformLocation("modelMatrix"      );
        location_shineDamper       = getUniformLocation("shineDamper"      );
        location_reflectivity      = getUniformLocation("reflectivity"     );
        location_skyColor          = getUniformLocation("skyColor"         );
        location_backgroundTexture = getUniformLocation("backgroundTexture");
        location_rTexture          = getUniformLocation("rTexture"         );
        location_gTexture          = getUniformLocation("gTexture"         );
        location_bTexture          = getUniformLocation("bTexture"         );
        location_blendMap          = getUniformLocation("blendMap"         );
        location_clipPlane         = getUniformLocation("clipPlane"        );

        for (int i = 0; i < MAX_LIGHTS; i++)
        {
            location_lightPosition[i] = getUniformLocation("lightPosition["+std::to_string(i)+"]");
            location_lightColor[i]    = getUniformLocation("lightColor["+std::to_string(i)+"]"   );
            location_attenuation[i]   = getUniformLocation("attenuation["+std::to_string(i)+"]"  );
        }
    }

    void TerrainShader::connectTextureUnits()
    {
        loadInt(location_backgroundTexture, 0);
        loadInt(location_rTexture, 1);
        loadInt(location_gTexture, 2);
        loadInt(location_bTexture, 3);
        loadInt(location_blendMap, 4);
    }

    void TerrainShader::SetProjectionMatrix(const glm::mat4& matrix)
    {
        loadMat4(location_projectionMatrix, matrix);
    }
    void TerrainShader::SetViewMatrix(const glm::mat4& matrix)
    {
        loadMat4(location_viewMatrix, matrix);
    }
    void TerrainShader::SetModelMatrix(const glm::mat4& matrix)
    {
        loadMat4(location_modelMatrix, matrix);
    }
    void TerrainShader::SetLights(const std::vector<const Light*>& lights)
    {
        for (int i = 0; i < MAX_LIGHTS; i++)
        {
            if (i < lights.size())
            {
                loadVec3(location_lightPosition[i], lights[i]->getPosition());
                loadVec3(location_lightColor[i], lights[i]->getColor());
                loadVec3(location_attenuation[i], lights[i]->getAttenuation());
            }
            else
            {
                loadVec3(location_lightPosition[i], {0,0,0});
                loadVec3(location_lightColor[i], {0,0,0});
                loadVec3(location_attenuation[i], {1,0,0});
            }
        }
    }
    void TerrainShader::SetShineVars(float damper, float reflect)
    {
        loadFloat(location_shineDamper, damper);
        loadFloat(location_reflectivity, reflect);
    }
    void TerrainShader::SetSkyColor(const glm::vec3 &col)
    {
        loadVec3(location_skyColor, col);
    }

    void TerrainShader::SetClipPlane(const glm::vec4 &pla)
    {
        loadVec4(location_clipPlane, pla);
    }

}
