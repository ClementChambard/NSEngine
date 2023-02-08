#include "AnimShaderTwoColor.h"

namespace NSEngine {

    const std::string AnimShaderTwoColor::VERTEX_SHADER_PATH = "assets/shaders/animTCVertexShader";
    const std::string AnimShaderTwoColor::FRAGMENT_SHADER_PATH = "assets/shaders/animTCFragmentShader";

    void AnimShaderTwoColor::bindAttributes()
    {
        bindAttribute(0, "vertexPosition");
        bindAttribute(1, "vertexTexCoords");
        bindAttribute(2, "color1");
        bindAttribute(3, "color2");
    }

    void AnimShaderTwoColor::getAllUniformLocation()
    {
        location_projectionMatrix = getUniformLocation("projectionMatrix");
        location_viewMatrix       = getUniformLocation("viewMatrix"      );
        location_cameraPosition   = getUniformLocation("cameraPosition"  );
        location_fogColor         = getUniformLocation("fogColor"        );
        location_fogMin           = getUniformLocation("fogMin"          );
        location_fogMax           = getUniformLocation("fogMax"          );
    }

    void AnimShaderTwoColor::SetProjectionMatrix(const glm::mat4& mat)
    {
        loadMat4(location_projectionMatrix, mat);
    }
    void AnimShaderTwoColor::SetViewMatrix(const glm::mat4& mat)
    {
        loadMat4(location_viewMatrix, mat);
    }
    void AnimShaderTwoColor::SetCameraPosition(const glm::vec3& vec)
    {
        loadVec3(location_cameraPosition, vec);
    }
    void AnimShaderTwoColor::SetFog(float min, float max, const glm::vec4& color)
    {
        loadFloat(location_fogMin, min);
        loadFloat(location_fogMax, max);
        loadVec4(location_fogColor, color);
    }
}
