#include "VerticalBlurShader.h"

namespace NSEngine {

    const std::string VerticalBlurShader::VERTEX_SHADER_PATH = "assets/engine/postProcessing/verticalBlurVertexShader";
    const std::string VerticalBlurShader::FRAGMENT_SHADER_PATH = "assets/engine/postProcessing/blurFragmentShader";

    void VerticalBlurShader::bindAttributes()
    {
        bindAttribute(0, "position");
    }

    void VerticalBlurShader::getAllUniformLocation()
    {
        location_targetWidth = getUniformLocation("targetWidth");
    }

    void VerticalBlurShader::SetTargetWidth(float w)
    {
        loadFloat(location_targetWidth, w);
    }

}
