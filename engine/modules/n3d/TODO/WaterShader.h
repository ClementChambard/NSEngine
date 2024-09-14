#ifndef WATERSHADER_HEADER_INCLUDED
#define WATERSHADER_HEADER_INCLUDED

#include <ShaderProgram.h>
#include "Light.h"

namespace ns::n3d {

class WaterShader : public ShaderProgram {
    public:
        WaterShader() : ShaderProgram(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH) { bind_attributes(); get_all_uniform_location(); }

        void set_projection_matrix(const mat4& mat);
        void set_view_matrix(const mat4& mat);
        void set_model_matrix(const mat4& mat);
        void set_camera_position(const vec3& pos);
        void set_move_factor(const vec2& mv);
        void set_sun(const Light* sun);

        void connect_textures();

    protected:
        void bind_attributes() override;
        void get_all_uniform_location() override;

    private:
        static cstr VERTEX_SHADER_PATH;
        static cstr FRAGMENT_SHADER_PATH;

        u32 location_projectionMatrix;
        u32 location_viewMatrix;
        u32 location_modelMatrix;
        u32 location_cameraPosition;
        u32 location_reflectionTexture;
        u32 location_refractionTexture;
        u32 location_dudvMap;
        u32 location_normalMap;
        u32 location_depthMap;
        u32 location_moveFactor;
        u32 location_lightPosition;
        u32 location_lightColor;
};

}

#endif // WATERSHADER_HEADER_INCLUDED
