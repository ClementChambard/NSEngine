#include "ShockwaveEffectShader.h"

#include <GL/glew.h>

namespace ns {

    cstr ShockwaveEffectShader::VERTEX_SHADER_PATH = "TODO/postproc.shockwaveEffect.vert.glsl";
    cstr ShockwaveEffectShader::FRAGMENT_SHADER_PATH = "TODO/postproc.shockwaveEffect.frag.glsl";

    void ShockwaveEffectShader::bind_attributes()
    {
        bind_attribute(0, "position");
    }

    void ShockwaveEffectShader::get_all_uniform_location()
    {
        location_mainTexture = get_uniform_location("tex" );
        location_waveTexture = get_uniform_location("tex2");
        location_aspectRatio = get_uniform_location("aspect");
        location_aberration = get_uniform_location("aberration");
        location_strength = get_uniform_location("strength");
    }

    void ShockwaveEffectShader::set_textures(float aspectRatio)
    {
        load_int(location_mainTexture, 0);
        load_int(location_waveTexture, 1);
        load_float(location_aspectRatio, aspectRatio);
    }

    void ShockwaveEffectShader::set_aberration(float a)
    {
        load_float(location_aberration, a);
    }

    void ShockwaveEffectShader::set_strength(float s)
    {
        load_float(location_strength, s);
    }

}
