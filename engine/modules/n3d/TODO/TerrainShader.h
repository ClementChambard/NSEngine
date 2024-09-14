#ifndef TERRAINSHADER_HEADER_INCLUDED
#define TERRAINSHADER_HEADER_INCLUDED

#include <ShaderProgram.h>
#include "Light.h"
#include <vector>

namespace ns::n3d {

class TerrainShader : public ns::ShaderProgram {
    public:
        TerrainShader() : ShaderProgram(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH) { bind_attributes(); get_all_uniform_location(); }
        
        void set_projection_matrix(const mat4& matrix);
        void set_view_matrix(const mat4& matrix);
        void set_model_matrix(const mat4& matrix);
        void set_lights(const std::vector<const Light*>& lights);
        void set_shine_vars(f32 damper, f32 reflect);
        void set_sky_color(const vec3& col);
        void connect_texture_units();
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
        u32 location_skyColor;
        u32 location_backgroundTexture;
        u32 location_rTexture;
        u32 location_gTexture;
        u32 location_bTexture;
        u32 location_blendMap;
        u32 location_clipPlane;
};

}

#endif // TERRAINSHADER_HEADER_INCLUDED
