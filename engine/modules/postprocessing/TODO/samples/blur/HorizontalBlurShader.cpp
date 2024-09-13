#include "HorizontalBlurShader.h"

namespace ns {

    cstr HorizontalBlurShader::VERTEX_SHADER_PATH = "TODO/postproc.horizontalBlur.vert.glsl";
    cstr HorizontalBlurShader::FRAGMENT_SHADER_PATH = "TODO/postproc.blur.frag.glsl";

    void HorizontalBlurShader::bind_attributes()
    {
        bind_attribute(0, "position");
    }

    void HorizontalBlurShader::get_all_uniform_location()
    {
        location_targetWidth = get_uniform_location("targetWidth");
    }

    void HorizontalBlurShader::set_target_width(f32 w)
    {
        load_float(location_targetWidth, w);
    }

}
