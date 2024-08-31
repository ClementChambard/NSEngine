#ifndef TEXTUREMANAGER_HEADER_INCLUDED
#define TEXTUREMANAGER_HEADER_INCLUDED

#include <vector>
#include "./Texture.hpp"
#include "./defines.h"

namespace ns {

class TextureManager {
    public:
        // TODO: cleanup before closing things to avoid problems
        static usize register_texture(cstr name);
        static usize add_texture(u32 tex_id, i32 w, i32 h);
        static usize add_texture(Texture&& t);
        static void get_texture_size(usize i, i32 &w, i32 &h);
        static u32 get_texture_id(usize i);
        static Texture* get_texture(usize i);
        static Texture* get_texture_by_opengl(u32 tex_id);
        static usize get_texture_nsid_by_opengl(u32 tex_id);
    private:
        static std::vector<Texture> s_textures;
        static usize s_currentTexture;
};

} // namespace ns

#endif // TEXTUREMANAGER_HEADER_INCLUDED
