#include "TextureManager.h"
#include "memory.h"

namespace ns {

std::vector<Texture> TextureManager::s_textures;
usize TextureManager::s_currentTexture = 0;

usize TextureManager::register_texture(cstr name) {
    if (s_textures.size() == 0) {
        s_textures.emplace_back();
    }
    s_textures.emplace_back(name);
    return s_textures.size() -1;
}

Texture* TextureManager::get_texture(usize i) { return &s_textures[i]; }

usize TextureManager::get_texture_nsid_by_opengl(u32 texID) {
    for (usize i = 0; i < s_textures.size(); i++) if (s_textures[i].get_opengl_id() == texID) return i;
    return 0;
}

usize TextureManager::add_texture(u32 texID, i32 w, i32 h) {
    Texture* t = Texture::from_opengl(texID, w, h);
    usize r = TextureManager::add_texture(std::move(*t));
    ns::free<MemTag::TEXTURE>(t);
    return r;
}

usize TextureManager::add_texture(Texture&& t) {
    s_textures.emplace_back(std::move(t));
    return s_textures.size() -1;
}

void TextureManager::get_texture_size(usize i, i32 &w, i32 &h) {
    auto s = s_textures[i].get_size();
    w = s.x;
    h = s.y;
}

u32 TextureManager::get_texture_id(usize i) {
    return s_textures[i].get_opengl_id();
}

Texture* TextureManager::get_texture_by_opengl(u32 texID) {
    for (auto& t : s_textures) if (t.get_opengl_id() == texID) return &t;
    return nullptr;
}

}



