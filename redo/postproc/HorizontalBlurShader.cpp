#include "HorizontalBlurShader.h"

namespace NSEngine {

    const std::string HorizontalBlurShader::VERTEX_SHADER_PATH = "assets/engine/postProcessing/horizontalBlurVertexShader";
    const std::string HorizontalBlurShader::FRAGMENT_SHADER_PATH = "assets/engine/postProcessing/blurFragmentShader";

    void HorizontalBlurShader::bindAttributes()
    {
        bindAttribute(0, "position");
    }

    void HorizontalBlurShader::getAllUniformLocation()
    {
        location_targetWidth = getUniformLocation("targetWidth");
    }

    void HorizontalBlurShader::SetTargetWidth(float w)
    {
        loadFloat(location_targetWidth, w);
    }

}
