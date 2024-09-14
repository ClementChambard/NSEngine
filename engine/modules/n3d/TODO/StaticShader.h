#ifndef STATICSHADER_HEADER_INCLUDED
#define STATICSHADER_HEADER_INCLUDED

#include <ShaderProgram.h>
#include "Light.h"
#include <vector>

namespace ns::n3d {

class StaticShader : public ShaderProgram {
    public:
        StaticShader() : ShaderProgram(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH) { bind_attributes(); get_all_uniform_location(); }
        
        void set_projection_matrix(const mat4& matrix);
        void set_view_matrix(const mat4& matrix);
        void set_model_matrix(const mat4& matrix);
        void set_lights(const std::vector<const Light*>& lights);
        void set_texture_vars(f32 damper, f32 reflect, bool use_fake_normals);
        void set_sky_color(const vec3& col);
        void set_number_of_rows(u32 num);
        void set_offset(const vec2& off);
        void set_clip_plane(const vec4& pla);

    protected:
        void bind_attributes() override;
        void get_all_uniform_location() override;

    private:
        static cstr VERTEX_SHADER_PATH;
        static cstr FRAGMENT_SHADER_PATH;

        u32 location_projectionMatrix;
        u32 location_viewMatrix;
        u32 location_modelMatrix;
        u32 location_lightPosition[Light::MAX_LIGHTS];
        u32 location_lightColor[Light::MAX_LIGHTS];
        u32 location_attenuation[Light::MAX_LIGHTS];
        u32 location_shineDamper;
        u32 location_reflectivity;
        u32 location_useFakeLighting;
        u32 location_skyColor;
        u32 location_numberOfRows;
        u32 location_offset;
        u32 location_clipPlane;
};

}

#endif // STATICSHADER_HEADER_INCLUDED
