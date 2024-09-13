#ifndef HORIZONTALBLUR_HEADER_INCLUDED
#define HORIZONTALBLUR_HEADER_INCLUDED

#include "../../PPEffect.h"
#include "HorizontalBlurShader.h"

namespace ns {

    class HorizontalBlur : public PPEffect {
        public:
            HorizontalBlur();
            HorizontalBlur(u32 target_fbo_width, u32 target_fbo_height);

            void cleanup() override { delete shader; delete renderer; }
            void render(u32 texture) override;
            u32 get_output_texture() override { return renderer->get_output_texture(); }

        private:
            HorizontalBlurShader* shader;
            ImageRenderer* renderer;
            
    };

}

#endif // HORIZONTALBLUR_HEADER_INCLUDED
