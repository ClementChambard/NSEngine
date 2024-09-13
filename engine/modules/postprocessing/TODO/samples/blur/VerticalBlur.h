#ifndef VERTICALBLUR_HEADER_INCLUDED
#define VERTICALBLUR_HEADER_INCLUDED

#include "../../PPEffect.h"
#include "VerticalBlurShader.h"

namespace ns {

    class VerticalBlur : public PPEffect {
        public:
            VerticalBlur();
            VerticalBlur(u32 target_fbo_width, u32 target_fbo_height);

            void cleanup() override { delete shader; delete renderer; }
            void render(u32 texture) override;
            u32 get_output_texture() override { return renderer->get_output_texture(); }

        private:
            VerticalBlurShader* shader;
            ImageRenderer* renderer;
            
    };

}

#endif // VERTICALBLUR_HEADER_INCLUDED
