#ifndef MODELTEXTURE_HEADER_INCLUDED
#define MODELTEXTURE_HEADER_INCLUDED

#include "Texture.hpp"
#include <defines.h>

namespace ns::n3d {

class ModelTexture {
    public:
        ModelTexture(Texture* tex) : tex(tex) {}

        Texture* get_texture() const { return tex; }
        f32 get_shine_damper() const { return shine_damper; }
        f32 get_reflectivity() const { return reflectivity; }
        bool get_has_transparency() const { return has_transparency; }
        bool get_use_fake_normals() const { return use_fake_normals; }
        u32 get_number_of_rows() const { return number_of_rows; }

        ModelTexture* set_shine_damper(const f32 sD) { shine_damper = sD; return this; }
        ModelTexture* set_reflectivity(const f32 r) { reflectivity = r; return this; }
        ModelTexture* set_specular(const f32 sD, const f32 r) { shine_damper = sD; reflectivity = r; return this; }
        ModelTexture* set_has_transparency(const bool b) { has_transparency = b; return this; }
        ModelTexture* set_use_fake_normals(const bool b) { use_fake_normals = b; return this; }
        ModelTexture* set_number_of_rows(const u32 n) { number_of_rows = n; return this; }

    private:
        Texture* tex = nullptr;
        f32 shine_damper = 1;
        f32 reflectivity = 0;
        bool has_transparency = false;
        bool use_fake_normals = false;
        u32 number_of_rows = 1;
};

}

#endif // MODELTEXTURE_HEADER_INCLUDED
