#ifndef PPEFFECT_HEADER_INCLUDED
#define PPEFFECT_HEADER_INCLUDED

#include "FrameBuffer.h"
#include "./defines.h"

namespace ns {

class PPEffect {
    public:
        virtual ~PPEffect() {}
        virtual void render(u32) {}
        virtual u32 get_output_texture() { return 0; }

        virtual void cleanup() {}

        class ImageRenderer {
            public:
                ImageRenderer(u32 w, u32 h);
                ImageRenderer();
                ~ImageRenderer();

                void render_quad();

                u32 get_output_texture() const;

                FrameBuffer* get_fbo() const;


            private:
                FrameBuffer* fbo = nullptr;

        };

};

}

#endif // PPEFFECT_HEADER_INCLUDED
