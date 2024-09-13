#ifndef HORIZONTALBLURSHADER_HEADER_INCLUDED
#define HORIZONTALBLURSHADER_HEADER_INCLUDED

#include <ShaderProgram.h>

namespace ns {

    class HorizontalBlurShader : public ShaderProgram {
        public:
            HorizontalBlurShader() : ShaderProgram(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH) { bind_attributes(); get_all_uniform_location(); }
        
            void set_target_width(f32 w);

        protected:
            void bind_attributes() override;
            void get_all_uniform_location() override;

        private:
            static cstr VERTEX_SHADER_PATH;
            static cstr FRAGMENT_SHADER_PATH;

            u32 location_targetWidth;
    };

}

#endif // HORIZONTALBLURSHADER_HEADER_INCLUDED