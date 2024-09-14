#include "SkyboxShader.h"

namespace ns::n3d {

    cstr SkyboxShader::VERTEX_SHADER_PATH = "assets/engine/shaders/skyboxVertexShader";
    cstr SkyboxShader::FRAGMENT_SHADER_PATH = "assets/engine/shaders/skyboxFragmentShader";

    void SkyboxShader::bind_attributes()
    {
        bind_attribute(0, "position");
    }

    void SkyboxShader::get_all_uniform_location()
    {
        location_projectionMatrix = get_uniform_location("projectionMatrix");
        location_viewMatrix       = get_uniform_location("viewMatrix"      );
    }

    void SkyboxShader::set_projection_matrix(const mat4& matrix)
    {
        load_mat4(location_projectionMatrix, matrix);
    }
    void SkyboxShader::set_view_matrix(const mat4& matrix)
    {
        mat4 m = matrix;
        m[3][0] = 0;
        m[3][1] = 0;
        m[3][2] = 0;
        load_mat4(location_viewMatrix, m);
    }
}
