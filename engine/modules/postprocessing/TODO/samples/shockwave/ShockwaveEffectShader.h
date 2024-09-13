#ifndef SHOCKWAVEEFFECTSHADER_HEADER_INCLUDED
#define SHOCKWAVEEFFECTSHADER_HEADER_INCLUDED

#include <ShaderProgram.h>

namespace ns {

    class ShockwaveEffectShader : public ShaderProgram {
        public:
            ShockwaveEffectShader() : ShaderProgram(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH) { bind_attributes(); get_all_uniform_location(); }

            void set_textures(f32 aspectRatio = 16/9.f);
            void set_aberration(f32 a);
            void set_strength(f32 s);

        protected:
            void bind_attributes() override;
            void get_all_uniform_location() override;

        private:
            static cstr VERTEX_SHADER_PATH;
            static cstr FRAGMENT_SHADER_PATH;

            i32 location_mainTexture;
            i32 location_waveTexture;
            i32 location_aspectRatio;
            i32 location_strength;
            i32 location_aberration;
    };

}

#endif // SHOCKWAVEEFFECTSHADER_HEADER_INCLUDED
