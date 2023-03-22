#include "SkyboxShader.h"

namespace NS3D {

    const std::string SkyboxShader::VERTEX_SHADER_PATH = "assets/engine/shaders/skyboxVertexShader";
    const std::string SkyboxShader::FRAGMENT_SHADER_PATH = "assets/engine/shaders/skyboxFragmentShader";

    void SkyboxShader::bindAttributes()
    {
        bindAttribute(0, "position");
    }

    void SkyboxShader::getAllUniformLocation()
    {
        location_projectionMatrix = getUniformLocation("projectionMatrix");
        location_viewMatrix       = getUniformLocation("viewMatrix"      );
    }

    void SkyboxShader::SetProjectionMatrix(const glm::mat4& matrix)
    {
        loadMat4(location_projectionMatrix, matrix);
    }
    void SkyboxShader::SetViewMatrix(const glm::mat4& matrix)
    {
        glm::mat4 m = matrix;
        m[3][0] = 0;
        m[3][1] = 0;
        m[3][2] = 0;
        loadMat4(location_viewMatrix, m);
    }
}
