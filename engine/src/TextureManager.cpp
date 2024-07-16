#include "TextureManager.h"
#include "FrameBuffer.h"
#include "memory.h"

#include <GL/glew.h>

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

void TextureManager::set_blendmode(i32 blendmode) {
    switch (blendmode) {
        case 0:
            glBlendEquation(GL_FUNC_ADD);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            break;
        case 1:
            glBlendEquation(GL_FUNC_ADD);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            break;
        case 2:
            glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            break;
        case 3:
            glBlendEquation(GL_FUNC_ADD);
            glBlendFunc(GL_ONE, GL_ZERO);
            break;
        case 4:
            glBlendEquation(GL_FUNC_ADD);
            glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE_MINUS_SRC_COLOR);
            break;
        case 5:
            glBlendEquation(GL_FUNC_ADD);
            glBlendFunc(GL_DST_COLOR, GL_ZERO);
            break;
        case 6:
            glBlendEquation(GL_FUNC_ADD);
            glBlendFunc(GL_ONE_MINUS_SRC_COLOR, GL_ONE_MINUS_SRC_ALPHA);
            break;
        case 7:
            glBlendEquation(GL_FUNC_ADD);
            glBlendFunc(GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA);
            break;
        case 8:
            glBlendEquation(GL_MIN);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            break;
        case 9:
            glBlendEquation(GL_MAX);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            break;
        case 10:
            glBlendEquation(GL_FUNC_SUBTRACT);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            break;
        default:
            glBlendEquation(GL_FUNC_ADD);
            glBlendFunc(GL_BLEND_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
}

void TextureManager::use_texture(usize i) {
    if (i == 0 || i >= s_textures.size()) return;
    s_textures[i].use();
}

void TextureManager::reset_texture() {
    Texture::unuse_texture();
}


usize TextureManager::get_texture_nsid_by_opengl(GLuint texID) {
    for (usize i = 0; i < s_textures.size(); i++) if (s_textures[i].get_opengl_id() == texID) return i;
    return 0;
}

usize TextureManager::add_texture(GLuint texID, i32 w, i32 h) {
    Texture* t = Texture::from_opengl(texID, w, h);
    usize r = TextureManager::add_texture(std::move(*t));
    ns::free(t, MemTag::TEXTURE);
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

GLuint TextureManager::get_texture_id(usize i) {
    return s_textures[i].get_opengl_id();
}

vec2 TextureManager::get_uv_at(usize i, i32 x, i32 y) {
    return s_textures[i].get_uv_at(x, y);
}

Texture* TextureManager::get_texture_by_opengl(GLuint texID) {
    for (auto& t : s_textures) if (t.get_opengl_id() == texID) return &t;
    return nullptr;
}

usize TextureManager::create_surface(i32 x, i32 y,
             [[maybe_unused]] float ratio) {
    Texture* t = Texture::as_framebuffer(x, y);
    usize r = TextureManager::add_texture(std::move(*t));
    ns::free(t, MemTag::TEXTURE);
    return r;
}

void TextureManager::set_surface_target(usize i,
                    i32 x1, i32 y1, i32 x2, i32 y2) {
    if (i >= s_textures.size()) return;
    if (s_textures[i].get_framebuffer() == nullptr) return;
    Texture::unuse_texture();
    s_textures[i].get_framebuffer()->bind();
    if (x1 == -1) {
        x1 = 0;
        y1 = 0;
        x2 = s_textures[i].get_width();
        y2 = s_textures[i].get_height();
    }
    glViewport(x1, y1, x2, y2);

    // TODO: If there is an "engine" camera, it should be changed here
}

GLuint TextureManager::get_surface_tex_id(usize i) {
    return s_textures[i].get_opengl_id();
}

}



