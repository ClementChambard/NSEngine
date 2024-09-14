#ifndef SKYBOXRENDERER_HEADER_INCLUDED
#define SKYBOXRENDERER_HEADER_INCLUDED

#include "RawModel.h"
#include "SkyboxShader.h"

namespace ns::n3d {

class SkyboxRenderer {
    public:
        SkyboxRenderer(SkyboxShader* shad);
        ~SkyboxRenderer() { delete shader; delete cube; }

        void render();

        void set_texture(u32 tex) { tex_id = tex; }

    private:
        static const f32 SIZE;
        static f32 VERTICES[];

        RawModel* cube;
        u32 tex_id = 0;
	SkyboxShader* shader;
};

}

#endif // SKYBOXRENDERER_HEADER_INCLUDED
