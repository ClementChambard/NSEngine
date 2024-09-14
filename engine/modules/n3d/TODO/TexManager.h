#ifndef TEXMANAGER_HEADER_INCLUDED
#define TEXMANAGER_HEADER_INCLUDED

#include <vector>
#include "TerrainTexture.h"
#include "ModelTexture.h"
#include <defines.h>
#include <Texture.hpp>

namespace ns::n3d {

class TexManager {
    public:
    
        static u32 add_model_texture(Texture* tex) {
            created_model_tex.push_back(new ModelTexture(tex));
            return created_model_tex.size() - 1;
        }

        static ModelTexture* gen_model_texture(Texture* tex) {
            return get_model_texture(add_model_texture(tex));
        }

        static ModelTexture* get_model_texture(u32 i) {
            return created_model_tex[i];
        }

        static u32 add_terrain_texture(Texture* tex_1, Texture* tex_2, Texture* tex_3, Texture* tex_4) {
            created_terrain_tex.push_back(new TerrainTexture(tex_1, tex_2, tex_3, tex_4));
            return created_terrain_tex.size() - 1;
        }

        static TerrainTexture* gen_terrain_texture(Texture* tex_1, Texture* tex_2, Texture* tex_3, Texture* tex_4) {
            return get_terrain_texture(add_terrain_texture(tex_1, tex_2, tex_3, tex_4));
        }

        static TerrainTexture* get_terrain_texture(u32 i) { return created_terrain_tex[i]; }

        static u32 add_gl_texture(cstr tex_file) {
            created_gl_tex.push_back(new Texture(tex_file));
            return created_gl_tex.size() - 1;
        }

        static Texture* gen_gl_texture(cstr tex_file) {
            return get_gl_texture(add_gl_texture(tex_file));
        }

        static Texture* get_gl_texture(u32 i) { return created_gl_tex[i]; }

        static void cleanup() {
            for (auto m : created_model_tex) delete m;
            for (auto t : created_terrain_tex) delete t;
            for (auto t : created_gl_tex) delete t;
        }

    private:
        static std::vector<Texture*> created_gl_tex;
        static std::vector<ModelTexture*> created_model_tex;
        static std::vector<TerrainTexture*> created_terrain_tex;
};

}

#endif // TEXMANAGER_HEADER_INCLUDED
