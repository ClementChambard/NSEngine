#include "TerrainShader.h"
#include <string>

namespace ns::n3d {

cstr TerrainShader::VERTEX_SHADER_PATH = "assets/engine/shaders/terrainVertexShader";
cstr TerrainShader::FRAGMENT_SHADER_PATH = "assets/engine/shaders/terrainFragmentShader";

void TerrainShader::bind_attributes()
{
    bind_attribute(0, "position");
    bind_attribute(1, "texCoords");
    bind_attribute(2, "normal");
}

void TerrainShader::get_all_uniform_location()
{
    location_projectionMatrix  = get_uniform_location("projectionMatrix" );
    location_viewMatrix        = get_uniform_location("viewMatrix"       );
    location_modelMatrix       = get_uniform_location("modelMatrix"      );
    location_shineDamper       = get_uniform_location("shineDamper"      );
    location_reflectivity      = get_uniform_location("reflectivity"     );
    location_skyColor          = get_uniform_location("skyColor"         );
    location_backgroundTexture = get_uniform_location("backgroundTexture");
    location_rTexture          = get_uniform_location("rTexture"         );
    location_gTexture          = get_uniform_location("gTexture"         );
    location_bTexture          = get_uniform_location("bTexture"         );
    location_blendMap          = get_uniform_location("blendMap"         );
    location_clipPlane         = get_uniform_location("clipPlane"        );

    for (size_t i = 0; i < Light::MAX_LIGHTS; i++)
    {
        location_lightPosition[i] = get_uniform_location(("lightPosition["+std::to_string(i)+"]").c_str());
        location_lightColor[i]    = get_uniform_location(("lightColor["+std::to_string(i)+"]"   ).c_str());
        location_attenuation[i]   = get_uniform_location(("attenuation["+std::to_string(i)+"]"  ).c_str());
    }
}

void TerrainShader::connect_texture_units()
{
    load_int(location_backgroundTexture, 0);
    load_int(location_rTexture, 1);
    load_int(location_gTexture, 2);
    load_int(location_bTexture, 3);
    load_int(location_blendMap, 4);
}

void TerrainShader::set_projection_matrix(const mat4& matrix)
{
    load_mat4(location_projectionMatrix, matrix);
}
void TerrainShader::set_view_matrix(const mat4& matrix)
{
    load_mat4(location_viewMatrix, matrix);
}
void TerrainShader::set_model_matrix(const mat4& matrix)
{
    load_mat4(location_modelMatrix, matrix);
}
void TerrainShader::set_lights(const std::vector<const Light*>& lights)
{
    for (size_t i = 0; i < Light::MAX_LIGHTS; i++)
    {
        if (i < lights.size())
        {
            load_vec3(location_lightPosition[i], lights[i]->get_position());
            load_vec3(location_lightColor[i], lights[i]->get_color());
            load_vec3(location_attenuation[i], lights[i]->get_attenuation());
        }
        else
        {
            load_vec3(location_lightPosition[i], {0,0,0});
            load_vec3(location_lightColor[i], {0,0,0});
            load_vec3(location_attenuation[i], {1,0,0});
        }
    }
}
void TerrainShader::set_shine_vars(f32 damper, f32 reflect)
{
    load_float(location_shineDamper, damper);
    load_float(location_reflectivity, reflect);
}
void TerrainShader::set_sky_color(const vec3 &col)
{
    load_vec3(location_skyColor, col);
}

void TerrainShader::set_clip_plane(const vec4 &pla)
{
    load_vec4(location_clipPlane, pla);
}

}
