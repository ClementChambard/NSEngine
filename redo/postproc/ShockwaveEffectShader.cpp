#include "ShockwaveEffectShader.h"

namespace NSEngine {

    const std::string ShockwaveEffectShader::VERTEX_SHADER_PATH = "assets/engine/postProcessing/shockwaveEffectVertexShader.vert";
    const std::string ShockwaveEffectShader::FRAGMENT_SHADER_PATH = "assets/engine/postProcessing/shockwaveEffectFragmentShader.frag";

    void ShockwaveEffectShader::bindAttributes()
    {
        bindAttribute(0, "position");
    }

    void ShockwaveEffectShader::getAllUniformLocation()
    {
        location_mainTexture = getUniformLocation("tex" );
        location_waveTexture = getUniformLocation("tex2");
        location_aspectRatio = getUniformLocation("aspect");
        location_aberration = getUniformLocation("aberration");
        location_strength = getUniformLocation("strength");
    }

    void ShockwaveEffectShader::setTextures(float aspectRatio)
    {
        glUniform1i(location_mainTexture, 0);
        glUniform1i(location_waveTexture, 1);
        glUniform1f(location_aspectRatio, aspectRatio);
    }

    void ShockwaveEffectShader::setAberration(float a)
    {
        glUniform1f(location_aberration, a);
    }

    void ShockwaveEffectShader::setStrength(float s)
    {
        glUniform1f(location_strength, s);
    }

}
