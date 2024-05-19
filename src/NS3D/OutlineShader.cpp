#include "OutlineShader.h"


namespace NS3D {

    cstr OutlineShader::VERTEX_SHADER_PATH = "assets/engine/shaders/outlineVertex";
    cstr OutlineShader::FRAGMENT_SHADER_PATH = "assets/engine/shaders/outlineFragment";

    void OutlineShader::bindAttributes()
    {
        bindAttribute(0, "position");
    }

    void OutlineShader::getAllUniformLocation()
    {
        location_projectionMatrix = getUniformLocation("projectionMatrix");
        location_viewMatrix       = getUniformLocation("viewMatrix"      );
        location_modelMatrix      = getUniformLocation("modelMatrix"     );
        location_skyColor         = getUniformLocation("skyColor"        );
        location_color            = getUniformLocation("color"           );
    }

    void OutlineShader::SetProjectionMatrix(const glm::mat4& matrix)
    {
        loadMat4(location_projectionMatrix, matrix);
    }
    void OutlineShader::SetViewMatrix(const glm::mat4& matrix)
    {
        loadMat4(location_viewMatrix, matrix);
    }
    void OutlineShader::SetModelMatrix(const glm::mat4& matrix)
    {
        loadMat4(location_modelMatrix, matrix);
    }
    void OutlineShader::SetSkyColor(const glm::vec3 &col)
    {
        loadVec3(location_skyColor, col);
    }
    void OutlineShader::SetColor(const glm::vec3 &col)
    {
        loadVec3(location_color, col);
    }

}
