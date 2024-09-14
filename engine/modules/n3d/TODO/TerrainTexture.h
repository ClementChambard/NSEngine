#ifndef TERRAINTEXTURE_HEADER_INCLUDED
#define TERRAINTEXTURE_HEADER_INCLUDED

#include <Texture.hpp>
#include <defines.h>

namespace ns::n3d {

class TerrainTexture {
    public:
        TerrainTexture(Texture* bg, Texture* r, Texture* g, Texture* b) :
            bg_tex(bg), r_tex(r), g_tex(g), b_tex(b) {}

        Texture* get_bg_tex() const { return bg_tex; }
        Texture* get_r_tex() const { return r_tex; }
        Texture* get_g_tex() const { return g_tex; }
        Texture* get_b_tex() const { return b_tex; }

    private:
        Texture* bg_tex;
        Texture* r_tex;
        Texture* g_tex;
        Texture* b_tex;
};

}

#endif // TERRAINTEXTURE_HEADER_INCLUDED
