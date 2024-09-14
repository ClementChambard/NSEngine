#include "WaterShader.h"

namespace ns::n3d {

cstr WaterShader::VERTEX_SHADER_PATH = "assets/engine/shaders/waterVertexShader";
cstr WaterShader::FRAGMENT_SHADER_PATH = "assets/engine/shaders/waterFragmentShader";

void WaterShader::bind_attributes()
{
    bind_attribute(0, "position");
}

void WaterShader::get_all_uniform_location()
{
    location_projectionMatrix  = get_uniform_location("projectionMatrix" );
    location_viewMatrix        = get_uniform_location("viewMatrix"       );
    location_modelMatrix       = get_uniform_location("modelMatrix"      );
    location_cameraPosition    = get_uniform_location("cameraPosition"   );
    location_reflectionTexture = get_uniform_location("reflectionTexture");
    location_refractionTexture = get_uniform_location("refractionTexture");
    location_dudvMap           = get_uniform_location("dudvMap"          );
    location_normalMap         = get_uniform_location("normalMap"        );
    location_depthMap          = get_uniform_location("depthMap"         );
    location_moveFactor        = get_uniform_location("moveFactor"       );
    location_lightPosition     = get_uniform_location("lightPosition"    );
    location_lightColor        = get_uniform_location("lightColor"       );
}

void WaterShader::connect_textures()
{
    load_int(location_reflectionTexture, 0);
    load_int(location_refractionTexture, 1);
    load_int(location_dudvMap, 2);
    load_int(location_normalMap, 3);
    load_int(location_depthMap, 4);
}

void WaterShader::set_projection_matrix(const mat4& matrix)
{
    load_mat4(location_projectionMatrix, matrix);
}
void WaterShader::set_view_matrix(const mat4& matrix)
{
    load_mat4(location_viewMatrix, matrix);
}
void WaterShader::set_model_matrix(const mat4& matrix)
{
    load_mat4(location_modelMatrix, matrix);
}
void WaterShader::set_camera_position(const vec3 &pos)
{
    load_vec3(location_cameraPosition, pos);
}

void WaterShader::set_move_factor(const vec2 &mv)
{
    load_vec2(location_moveFactor, mv);
}

void WaterShader::set_sun(const Light* sun)
{
    load_vec3(location_lightPosition, sun->get_position());
    load_vec3(location_lightColor, sun->get_color());
}

}
