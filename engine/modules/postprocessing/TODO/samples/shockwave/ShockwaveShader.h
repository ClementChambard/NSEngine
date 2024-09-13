#ifndef SHOCKWAVESHADER_HEADER_INCLUDED
#define SHOCKWAVESHADER_HEADER_INCLUDED

#include <ShaderProgram.h>

namespace ns {

    class ShockwaveShader : public ShaderProgram {
        public:
            ShockwaveShader() : ShaderProgram(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH) { bind_attributes(); get_all_uniform_location(); }

            void set_attenuation(f32 a);

        protected:
            void bind_attributes() override;
            void get_all_uniform_location() override;

        private:
            static cstr VERTEX_SHADER_PATH;
            static cstr FRAGMENT_SHADER_PATH;

            u32 location_attenuation;
    };

}


#endif // SHOCKWAVESHADER_HEADER_INCLUDED
