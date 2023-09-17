#include "TextureManager.h"
#include "NSEngine.h"
#include "Engine.hpp"
#include "FrameBuffer.h"

#include <iostream>

#include "FileOpener.h"

namespace NSEngine {

    std::vector<Texture> TextureManager::textures;
    size_t TextureManager::currentTexture = 0;

    size_t TextureManager::RegisterTexture(const char* name) {
        if (textures.size() == 0) {
            textures.emplace_back();
        }
        textures.emplace_back(name);
        return textures.size() -1;
    }

    Texture* TextureManager::GetTexture(size_t i) { return &textures[i]; }

    void TextureManager::SetBlendmode(int blendmode) {
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

    void TextureManager::UseTexture(size_t i) {
        if (i == 0 || i >= textures.size()) return;
        textures[i].use();
    }

    void TextureManager::ResetTexture() {
        Texture::unuseTexture();
    }


    size_t TextureManager::GetTextureNSIDByOpengl(GLuint texID) {
        for (size_t i = 0; i < textures.size(); i++) if (textures[i].getOpenglId() == texID) return i;
        return 0;
    }

    size_t TextureManager::AddTexture(GLuint texID, int w, int h) {
        Texture* t = Texture::fromOpenGL(texID, w, h);
        size_t r = TextureManager::AddTexture(std::move(*t));
        delete t;
        return r;
    }

    size_t TextureManager::AddTexture(Texture&& t) {
        textures.emplace_back(std::move(t));
        return textures.size() -1;
    }

    void TextureManager::GetTextureSize(size_t i, int &w, int &h) {
        auto s = textures[i].getSize();
        w = s.x;
        h = s.y;
    }

    GLuint TextureManager::GetTextureID(size_t i) {
        return textures[i].getOpenglId();
    }

    glm::vec2 TextureManager::GetUVAt(size_t i, int x, int y) {
        return textures[i].getUvAt(x, y);
    }

    Texture* TextureManager::GetTextureByOpengl(GLuint texID) {
        for (auto& t : textures) if (t.getOpenglId() == texID) return &t;
        return nullptr;
    }

    size_t TextureManager::CreateSurface(int x, int y,
                 [[maybe_unused]] float ratio) {
        Texture* t = Texture::asFramebuffer(x, y);
        size_t r = TextureManager::AddTexture(std::move(*t));
        delete t;
        return r;
    }

    void TextureManager::SetSurfaceTarget(size_t i,
                        int x1, int y1, int x2, int y2) {
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
        setCamBoundaries(x2-x1, y2-y1);
    }

    GLuint TextureManager::GetSurfaceTexID(size_t i) {
        return textures[i].getOpenglId();
    }

}



