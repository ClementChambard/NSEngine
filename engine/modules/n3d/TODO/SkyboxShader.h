#ifndef SKYBOXSHADER_HEADER_INCLUDED
#define SKYBOXSHADER_HEADER_INCLUDED

#include <ShaderProgram.h>

namespace ns::n3d {

class SkyboxShader : public ns::ShaderProgram {
    public:
       SkyboxShader() : ShaderProgram(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH) { bind_attributes(); get_all_uniform_location(); }

       void set_projection_matrix(const mat4& mat);
       void set_view_matrix(const mat4& mat);

    protected:
        void bind_attributes() override;
        void get_all_uniform_location() override;

    private:
        static cstr VERTEX_SHADER_PATH;
        static cstr FRAGMENT_SHADER_PATH;

        u32 location_projectionMatrix;
        u32 location_viewMatrix;
};

}

#endif // SKYBOXSHADER_HEADER_INCLUDED
