#include "ShockwaveShader.h"

namespace NSEngine {

    const std::string ShockwaveShader::VERTEX_SHADER_PATH = "assets/engine/shaders/shockwaveVertexShader.vert";
    const std::string ShockwaveShader::FRAGMENT_SHADER_PATH = "assets/engine/shaders/shockwaveFragmentShader.frag";

    void ShockwaveShader::bindAttributes()
    {
        bindAttribute(0, "vertexPosition");
        bindAttribute(1, "vertexTexCoords");
    }

    void ShockwaveShader::getAllUniformLocation()
    {
        location_attenuation = getUniformLocation("attenuation");
    }

    void ShockwaveShader::SetAttenuation(float a)
    {
        loadFloat(location_attenuation, a);
    }

}
