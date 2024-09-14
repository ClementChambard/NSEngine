#include "StaticShader.h"
#include <string>

namespace ns::n3d {

    cstr StaticShader::VERTEX_SHADER_PATH = "assets/engine/shaders/vertexShader";
    cstr StaticShader::FRAGMENT_SHADER_PATH = "assets/engine/shaders/fragmentShader";

    void StaticShader::bind_attributes()
    {
        bind_attribute(0, "position");
        bind_attribute(1, "texCoords");
        bind_attribute(2, "normal");
    }

    void StaticShader::get_all_uniform_location()
    {
        location_projectionMatrix = get_uniform_location("projectionMatrix");
        location_viewMatrix       = get_uniform_location("viewMatrix"      );
        location_modelMatrix      = get_uniform_location("modelMatrix"     );
        location_shineDamper      = get_uniform_location("shineDamper"     );
        location_reflectivity     = get_uniform_location("reflectivity"    );
        location_useFakeLighting  = get_uniform_location("useFakeLighting" );
        location_skyColor         = get_uniform_location("skyColor"        );
        location_numberOfRows     = get_uniform_location("numberOfRows"    );
        location_offset           = get_uniform_location("offset"          );
        location_clipPlane        = get_uniform_location("clipPlane"       );

        for (size_t i = 0; i < Light::MAX_LIGHTS; i++)
        {
            location_lightPosition[i] = get_uniform_location(("lightPosition["+std::to_string(i)+"]").c_str());
            location_lightColor[i]    = get_uniform_location(("lightColor["+std::to_string(i)+"]"   ).c_str());
            location_attenuation[i]   = get_uniform_location(("attenuation["+std::to_string(i)+"]"  ).c_str());
        }
    }

    void StaticShader::set_projection_matrix(const mat4& matrix)
    {
        load_mat4(location_projectionMatrix, matrix);
    }
    void StaticShader::set_view_matrix(const mat4& matrix)
    {
        load_mat4(location_viewMatrix, matrix);
    }
    void StaticShader::set_model_matrix(const mat4& matrix)
    {
        load_mat4(location_modelMatrix, matrix);
    }
    void StaticShader::set_lights(const std::vector<const Light*>& lights)
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
    void StaticShader::set_texture_vars(float damper, float reflect, bool use_fake_normals)
    {
        load_float(location_shineDamper, damper);
        load_float(location_reflectivity, reflect);
        load_bool(location_useFakeLighting, use_fake_normals);
    }
    void StaticShader::set_sky_color(const vec3 &col)
    {
        load_vec3(location_skyColor, col);
    }
    void StaticShader::set_number_of_rows(u32 num)
    {
        load_int(location_numberOfRows, num);
    }
    void StaticShader::set_offset(const vec2& off)
    {
        load_vec2(location_offset, off);
    }

    void StaticShader::set_clip_plane(const vec4 &pla)
    {
        load_vec4(location_clipPlane, pla);
    }

}
