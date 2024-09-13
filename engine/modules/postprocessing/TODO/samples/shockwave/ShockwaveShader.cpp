#include "ShockwaveShader.h"

namespace ns {

    cstr ShockwaveShader::VERTEX_SHADER_PATH = "TODO/postproc.shockwave.vert.glsl";
    cstr ShockwaveShader::FRAGMENT_SHADER_PATH = "TODO/postproc.shockwave.frag.glsl";

    void ShockwaveShader::bind_attributes()
    {
        bind_attribute(0, "vertexPosition");
        bind_attribute(1, "vertexTexCoords");
    }

    void ShockwaveShader::get_all_uniform_location()
    {
        location_attenuation = get_uniform_location("attenuation");
    }

    void ShockwaveShader::set_attenuation(float a)
    {
        load_float(location_attenuation, a);
    }

}
