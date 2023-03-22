#ifndef VERTICALBLUR_INCLUDED_H
#define VERTICALBLUR_INCLUDED_H

#include "PPEffect.h"
#include "VerticalBlurShader.h"

namespace NSEngine {

    class VerticalBlur : public PPEffect {
        public:
            VerticalBlur();
            VerticalBlur(int targetFboWidth, int targetFboHeight);

            void cleanup() override { delete shader; delete renderer; }
            void render(GLuint texture) override;
            GLuint getOutputTexture() override { return renderer->getOutputTexture(); }

        private:
            VerticalBlurShader* shader;
            ImageRenderer* renderer;
            
    };

}

#endif
