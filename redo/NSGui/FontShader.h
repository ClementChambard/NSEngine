#ifndef FONTSHADER_INCLUDED_H
#define FONTSHADER_INCLUDED_H

#include "../ShaderProgram.h"

namespace NSGui {

    class FontShader : public NSEngine::ShaderProgram {
        public:
            FontShader() : ShaderProgram(VERTEX_SHADER_FILE, FRAGMENT_SHADER_FILE) { bindAttributes(); getAllUniformLocation(); }

        protected:
            void bindAttributes() override;
            void getAllUniformLocation() override;

        private:
            static const std::string VERTEX_SHADER_FILE;
            static const std::string FRAGMENT_SHADER_FILE;
    };

}

#endif
