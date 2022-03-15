#ifndef HORIZONTALBLUR_INCLUDED_H
#define HORIZONTALBLUR_INCLUDED_H

#include "PPEffect.h"
#include "HorizontalBlurShader.h"

namespace NSEngine {

    class HorizontalBlur : public PPEffect {
        public:
            HorizontalBlur();
            HorizontalBlur(int targetFboWidth, int targetFboHeight);

            void cleanup() override { delete shader; delete renderer; }
            void render(GLuint texture) override;
            GLuint getOutputTexture() override { return renderer->getOutputTexture(); }

        private:
            HorizontalBlurShader* shader;
            ImageRenderer* renderer;
            
    };

}

#endif
