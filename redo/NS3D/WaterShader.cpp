#include "WaterShader.h"

namespace NS3D {

    cstr WaterShader::VERTEX_SHADER_PATH = "assets/engine/shaders/waterVertexShader";
    cstr WaterShader::FRAGMENT_SHADER_PATH = "assets/engine/shaders/waterFragmentShader";

    void WaterShader::bindAttributes()
    {
        bindAttribute(0, "position");
    }

    void WaterShader::getAllUniformLocation()
    {
        location_projectionMatrix  = getUniformLocation("projectionMatrix" );
        location_viewMatrix        = getUniformLocation("viewMatrix"       );
        location_modelMatrix       = getUniformLocation("modelMatrix"      );
        location_cameraPosition    = getUniformLocation("cameraPosition"   );
        location_reflectionTexture = getUniformLocation("reflectionTexture");
        location_refractionTexture = getUniformLocation("refractionTexture");
        location_dudvMap           = getUniformLocation("dudvMap"          );
        location_normalMap         = getUniformLocation("normalMap"        );
        location_depthMap          = getUniformLocation("depthMap"         );
        location_moveFactor        = getUniformLocation("moveFactor"       );
        location_lightPosition     = getUniformLocation("lightPosition"    );
        location_lightColor        = getUniformLocation("lightColor"       );
    }

    void WaterShader::connectTextures()
    {
        loadInt(location_reflectionTexture, 0);
        loadInt(location_refractionTexture, 1);
        loadInt(location_dudvMap, 2);
        loadInt(location_normalMap, 3);
        loadInt(location_depthMap, 4);
    }

    void WaterShader::SetProjectionMatrix(const glm::mat4& matrix)
    {
        loadMat4(location_projectionMatrix, matrix);
    }
    void WaterShader::SetViewMatrix(const glm::mat4& matrix)
    {
        loadMat4(location_viewMatrix, matrix);
    }
    void WaterShader::SetModelMatrix(const glm::mat4& matrix)
    {
        loadMat4(location_modelMatrix, matrix);
    }
    void WaterShader::SetCameraPosition(const glm::vec3 &pos)
    {
        loadVec3(location_cameraPosition, pos);
    }
    void WaterShader::SetMoveFactor(const glm::vec2 &mv)
    {
        loadVec2(location_moveFactor, mv);
    }
    void WaterShader::SetSun(const Light* sun)
    {
        loadVec3(location_lightPosition, sun->getPosition());
        loadVec3(location_lightColor, sun->getColor());
    }
}
