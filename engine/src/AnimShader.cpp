#include "AnimShader.h"

namespace ns {

cstr AnimShader::VERTEX_SHADER_PATH = "assets/engine/shaders/animVertexShader";
cstr AnimShader::FRAGMENT_SHADER_PATH = "assets/engine/shaders/animFragmentShader";

void AnimShader::bindAttributes()
{
    bindAttribute(0, "vertexPosition");
    bindAttribute(1, "vertexColor");
    bindAttribute(2, "vertexTexCoords");
}

void AnimShader::getAllUniformLocation()
{
    location_projectionMatrix = getUniformLocation("projectionMatrix");
    location_viewMatrix       = getUniformLocation("viewMatrix"      );
    location_cameraPosition   = getUniformLocation("cameraPosition"  );
    location_fogColor         = getUniformLocation("fogColor"        );
    location_fogMin           = getUniformLocation("fogMin"          );
    location_fogMax           = getUniformLocation("fogMax"          );
}

void AnimShader::SetProjectionMatrix(const glm::mat4& mat)
{
    loadMat4(location_projectionMatrix, mat);
}
void AnimShader::SetViewMatrix(const glm::mat4& mat)
{
    loadMat4(location_viewMatrix, mat);
}
void AnimShader::SetCameraPosition(const glm::vec3& vec)
{
    loadVec3(location_cameraPosition, vec);
}
void AnimShader::SetFog(f32 min, f32 max, const glm::vec4& color)
{
    loadFloat(location_fogMin, min);
    loadFloat(location_fogMax, max);
    loadVec4(location_fogColor, color);
}


}
