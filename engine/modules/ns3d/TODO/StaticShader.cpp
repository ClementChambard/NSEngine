#include "StaticShader.h"

#include <string>

namespace NS3D {

    cstr StaticShader::VERTEX_SHADER_PATH = "assets/engine/shaders/vertexShader";
    cstr StaticShader::FRAGMENT_SHADER_PATH = "assets/engine/shaders/fragmentShader";

    void StaticShader::bindAttributes()
    {
        bindAttribute(0, "position");
        bindAttribute(1, "texCoords");
        bindAttribute(2, "normal");
    }

    void StaticShader::getAllUniformLocation()
    {
        location_projectionMatrix = getUniformLocation("projectionMatrix");
        location_viewMatrix       = getUniformLocation("viewMatrix"      );
        location_modelMatrix      = getUniformLocation("modelMatrix"     );
        location_shineDamper      = getUniformLocation("shineDamper"     );
        location_reflectivity     = getUniformLocation("reflectivity"    );
        location_useFakeLighting  = getUniformLocation("useFakeLighting" );
        location_skyColor         = getUniformLocation("skyColor"        );
        location_numberOfRows     = getUniformLocation("numberOfRows"    );
        location_offset           = getUniformLocation("offset"          );
        location_clipPlane        = getUniformLocation("clipPlane"       );

        for (size_t i = 0; i < MAX_LIGHTS; i++)
        {
            location_lightPosition[i] = getUniformLocation(("lightPosition["+std::to_string(i)+"]").c_str());
            location_lightColor[i]    = getUniformLocation(("lightColor["+std::to_string(i)+"]"   ).c_str());
            location_attenuation[i]   = getUniformLocation(("attenuation["+std::to_string(i)+"]"  ).c_str());
        }
    }

    void StaticShader::SetProjectionMatrix(const glm::mat4& matrix)
    {
        loadMat4(location_projectionMatrix, matrix);
    }
    void StaticShader::SetViewMatrix(const glm::mat4& matrix)
    {
        loadMat4(location_viewMatrix, matrix);
    }
    void StaticShader::SetModelMatrix(const glm::mat4& matrix)
    {
        loadMat4(location_modelMatrix, matrix);
    }
    void StaticShader::SetLights(const std::vector<const Light*>& lights)
    {
        for (size_t i = 0; i < MAX_LIGHTS; i++)
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
    void StaticShader::SetTextureVars(float damper, float reflect, bool useFakeNormals)
    {
        loadFloat(location_shineDamper, damper);
        loadFloat(location_reflectivity, reflect);
        loadBool(location_useFakeLighting, useFakeNormals);
    }
    void StaticShader::SetSkyColor(const glm::vec3 &col)
    {
        loadVec3(location_skyColor, col);
    }
    void StaticShader::SetNumberOfRows(int num)
    {
        loadInt(location_numberOfRows, num);
    }
    void StaticShader::SetOffset(const glm::vec2& off)
    {
        loadVec2(location_offset, off);
    }

    void StaticShader::SetClipPlane(const glm::vec4 &pla)
    {
        loadVec4(location_clipPlane, pla);
    }

}
