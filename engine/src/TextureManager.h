#ifndef TEXTUREMANAGER_HEADER_INCLUDED
#define TEXTUREMANAGER_HEADER_INCLUDED

#include <vector>
#include "./Texture.hpp"
#include "./defines.h"

namespace ns {

enum Blendmode {bm_normal = 0, bm_add, bm_substract};

class TextureManager {
    public:
        // TODO: cleanup before closing things to avoid problems
        static usize register_texture(cstr name);
        static void use_texture(usize i);
        static void set_blendmode(i32 i);
        static void reset_texture();
        static usize add_texture(u32 tex_id, i32 w, i32 h);
        static usize add_texture(Texture&& t);
        static void get_texture_size(usize i, i32 &w, i32 &h);
        static u32 get_texture_id(usize i);
        static Texture* get_texture(usize i);
        static Texture* get_texture_by_opengl(u32 tex_id);
        static usize get_texture_nsid_by_opengl(u32 tex_id);
        static vec2 get_uv_at(usize i, i32 x, i32 y);
        static usize create_surface(i32 x, i32 y, f32 ratio = 1);
        static void set_surface_target(usize i, i32 x1 = -1, i32 y1 = -1, i32 x2 = -1, i32 y2 = -1);
        static u32 get_surface_tex_id(usize i);
    private:
        static std::vector<Texture> s_textures;
        static usize s_currentTexture;
};

} // namespace ns

#endif // TEXTUREMANAGER_HEADER_INCLUDED
