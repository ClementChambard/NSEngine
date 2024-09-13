#include "VerticalBlurShader.h"

namespace ns {

    cstr VerticalBlurShader::VERTEX_SHADER_PATH = "TODO/postproc.verticalBlur.vert.glsl";
    cstr VerticalBlurShader::FRAGMENT_SHADER_PATH = "TODO/postproc.blur.frag.glsl";

    void VerticalBlurShader::bind_attributes()
    {
        bind_attribute(0, "position");
    }

    void VerticalBlurShader::get_all_uniform_location()
    {
        location_targetWidth = get_uniform_location("targetWidth");
    }

    void VerticalBlurShader::set_target_width(float w)
    {
        load_float(location_targetWidth, w);
    }

}
