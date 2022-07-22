#ifndef SHOCKWAVEEFFECTSHADER_H_
#define SHOCKWAVEEFFECTSHADER_H_

#include "ShaderProgram.h"

namespace NSEngine {

    class ShockwaveEffectShader : public ShaderProgram {
        public:
            ShockwaveEffectShader() : ShaderProgram(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH) { bindAttributes(); getAllUniformLocation(); }

            void setTextures(float aspectRatio = 16/9.f);
            void setAberration(float a);
            void setStrength(float s);

        protected:
            void bindAttributes() override;
            void getAllUniformLocation() override;

        private:
            static const std::string VERTEX_SHADER_PATH;
            static const std::string FRAGMENT_SHADER_PATH;

            GLint location_mainTexture;
            GLint location_waveTexture;
            GLint location_aspectRatio;
            GLint location_strength;
            GLint location_aberration;
    };

}

#endif // SHOCKWAVEEFFECTSHADER_H_
