#ifndef HORIZONTALBLURSHADER_INCLUDED_H
#define HORIZONTALBLURSHADER_INCLUDED_H

#include "ShaderProgram.h"

namespace NSEngine {

    class HorizontalBlurShader : public ShaderProgram {
        public:
            HorizontalBlurShader() : ShaderProgram(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH) { bindAttributes(); getAllUniformLocation(); }
        
            void SetTargetWidth(float w);

        protected:
            void bindAttributes() override;
            void getAllUniformLocation() override;

        private:
            static const std::string VERTEX_SHADER_PATH;
            static const std::string FRAGMENT_SHADER_PATH;

            GLuint location_targetWidth;
    };

}

#endif
