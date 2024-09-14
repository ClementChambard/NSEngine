#ifndef OUTLINESHADER_HEADER_INCLUDED
#define OUTLINESHADER_HEADER_INCLUDED

#include <ShaderProgram.h>

namespace ns::n3d {

    class OutlineShader : public ShaderProgram {
        public:
            OutlineShader() : ShaderProgram(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH) { bind_attributes(); get_all_uniform_location(); }

            void set_projection_matrix(const mat4& matrix);
            void set_view_matrix(const mat4& matrix);
            void set_model_matrix(const mat4& matrix);
            void set_sky_color(const vec3& col);
            void set_color(const vec3& col);

        protected:
            void bind_attributes() override;
            void get_all_uniform_location() override;

        private:
            static cstr VERTEX_SHADER_PATH;
            static cstr FRAGMENT_SHADER_PATH;

            u32 location_projectionMatrix;
            u32 location_viewMatrix;
            u32 location_modelMatrix;
            u32 location_skyColor;
            u32 location_color;
    };

}


#endif // OUTLINESHADER_HEADER_INCLUDED
