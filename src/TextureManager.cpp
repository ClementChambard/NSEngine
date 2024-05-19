#include "TextureManager.h"
#include "NSEngine.hpp"
#include "FrameBuffer.h"
#include "Camera3D.h"

namespace ns {

Camera3D* activeCamera3D();

std::vector<Texture> TextureManager::textures;
usize TextureManager::currentTexture = 0;

usize TextureManager::RegisterTexture(cstr name) {
    if (textures.size() == 0) {
        textures.emplace_back();
    }
    textures.emplace_back(name);
    return textures.size() -1;
}

Texture* TextureManager::GetTexture(usize i) { return &textures[i]; }

void TextureManager::SetBlendmode(i32 blendmode) {
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

void TextureManager::UseTexture(usize i) {
    if (i == 0 || i >= textures.size()) return;
    textures[i].use();
}

void TextureManager::ResetTexture() {
    Texture::unuseTexture();
}


usize TextureManager::GetTextureNSIDByOpengl(GLuint texID) {
    for (usize i = 0; i < textures.size(); i++) if (textures[i].getOpenglId() == texID) return i;
    return 0;
}

usize TextureManager::AddTexture(GLuint texID, i32 w, i32 h) {
    Texture* t = Texture::fromOpenGL(texID, w, h);
    usize r = TextureManager::AddTexture(std::move(*t));
    delete t;
    return r;
}

usize TextureManager::AddTexture(Texture&& t) {
    textures.emplace_back(std::move(t));
    return textures.size() -1;
}

void TextureManager::GetTextureSize(usize i, i32 &w, i32 &h) {
    auto s = textures[i].getSize();
    w = s.x;
    h = s.y;
}

GLuint TextureManager::GetTextureID(usize i) {
    return textures[i].getOpenglId();
}

glm::vec2 TextureManager::GetUVAt(usize i, i32 x, i32 y) {
    return textures[i].getUvAt(x, y);
}

Texture* TextureManager::GetTextureByOpengl(GLuint texID) {
    for (auto& t : textures) if (t.getOpenglId() == texID) return &t;
    return nullptr;
}

usize TextureManager::CreateSurface(i32 x, i32 y,
             [[maybe_unused]] float ratio) {
    Texture* t = Texture::asFramebuffer(x, y);
    usize r = TextureManager::AddTexture(std::move(*t));
    delete t;
    return r;
}

void TextureManager::SetSurfaceTarget(usize i,
                    i32 x1, i32 y1, i32 x2, i32 y2) {
    if (i >= textures.size()) return;
    if (textures[i].getFramebuffer() == nullptr) return;
    Texture::unuseTexture();
    textures[i].getFramebuffer()->bind();
    if (x1 == -1) {
        x1 = 0;
        y1 = 0;
        x2 = textures[i].getWidth();
        y2 = textures[i].getHeight();
    }
    glViewport(x1, y1, x2, y2);

    auto data = getInstance()->window().getWindowData();
    if (activeCamera3D() != nullptr) activeCamera3D()->setWH(x2-x1, y2-y1, (float)(x2-x1)/(float)data.bwidth);
}

GLuint TextureManager::GetSurfaceTexID(usize i) {
    return textures[i].getOpenglId();
}

}



