#include "OutlineShader.h"


namespace ns::n3d {

    cstr OutlineShader::VERTEX_SHADER_PATH = "assets/engine/shaders/outlineVertex";
    cstr OutlineShader::FRAGMENT_SHADER_PATH = "assets/engine/shaders/outlineFragment";

    void OutlineShader::bind_attributes()
    {
        bind_attribute(0, "position");
    }

    void OutlineShader::get_all_uniform_location()
    {
        location_projectionMatrix = get_uniform_location("projectionMatrix");
        location_viewMatrix       = get_uniform_location("viewMatrix"      );
        location_modelMatrix      = get_uniform_location("modelMatrix"     );
        location_skyColor         = get_uniform_location("skyColor"        );
        location_color            = get_uniform_location("color"           );
    }

    void OutlineShader::set_projection_matrix(const mat4& matrix)
    {
        load_mat4(location_projectionMatrix, matrix);
    }
    void OutlineShader::set_view_matrix(const mat4& matrix)
    {
        load_mat4(location_viewMatrix, matrix);
    }
    void OutlineShader::set_model_matrix(const mat4& matrix)
    {
        load_mat4(location_modelMatrix, matrix);
    }
    void OutlineShader::set_sky_color(const vec3 &col)
    {
        load_vec3(location_skyColor, col);
    }
    void OutlineShader::set_color(const vec3 &col)
    {
        load_vec3(location_color, col);
    }

}
