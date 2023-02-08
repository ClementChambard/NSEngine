#include "TextureManager.h"
#include "NSEngine.h"

#include <iostream>

#include "FileOpener.h"

namespace NSEngine {

    std::vector<GLTexture> TextureManager::textures;
    std::vector<GLSurface> TextureManager::surfaces;
    int TextureManager::currentTexture = 0;

    int TextureManager::RegisterTexture(const char* name, const char* name2, const char* name3, const char* name4)
    {
     //   for (int i = 0; i < textures.size(); i++) if (textures[i].name == name) return i;
        if (textures.size() == 0)
        {
            textures.push_back({0,0,0,0,0,0,"____EMPTY____"});
        }
        textures.push_back(ImageLoader::loadImageFile(name,name2,name3,name4));
        textures.back().name = name;
        return textures.size() -1;
    }

    GLTexture* TextureManager::GetTexture(int i) { return &textures[i]; }

    void TextureManager::SetBlendmode(int blendmode)
    {
        switch (blendmode)
        {
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
                //
                glBlendFunc(GL_SRC_ALPHA,GL_ONE);
                break;
            default:
                glBlendEquation(GL_FUNC_ADD);
                glBlendFunc(GL_BLEND_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }
    }

    void TextureManager::UseTexture(int i)
    {
        if (currentTexture != i) {
            if (engineData::displaymode == 3 && textures[i].id4!=0)
                glBindTexture(GL_TEXTURE_2D, textures[i].id4);
            else if (engineData::displaymode == 2 && textures[i].id3!=0)
                glBindTexture(GL_TEXTURE_2D, textures[i].id3);
            else if (engineData::displaymode == 1 && textures[i].id2!=0)
                glBindTexture(GL_TEXTURE_2D, textures[i].id2);
            else
                glBindTexture(GL_TEXTURE_2D, textures[i].id);

            engineData::fps->currentTexSwap++;
        }
    }

    void TextureManager::ResetTexture()
    {
        if (currentTexture != 0) glBindTexture(GL_TEXTURE_2D, 0);
    }

    int TextureManager::AddTexture(GLuint texID, int w, int h)
    {
        textures.push_back({texID, w, h, 0, 0, 0, "____UNKNOWN_NAME____"});
        return textures.size() -1;
    }

    void TextureManager::GetTextureSize(int i, int &w, int &h)
    {
        w = textures[i].width;
        h = textures[i].height;
    }

    GLuint TextureManager::GetTextureID(int i)
    {
        return textures[i].id;
    }

    glm::vec2 TextureManager::GetUVAt(int i, int x, int y)
    {
        return {(float)x / (float)textures[i].width, (float)y / (float)textures[i].height};
    }

    int TextureManager::CreateSurface(int x, int y, float ratio)
    {
        int r = surfaces.size();
        GLuint fb = 0; glGenFramebuffers(1,&fb);
        GLTexture tex = ImageLoader::emptyTexture(x,y);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fb);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,tex.id,0);
        GLuint rb = 0; glGenRenderbuffers(1, &rb);
        glBindRenderbuffer(GL_RENDERBUFFER,rb);
        glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT,x,y);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rb);
        GLenum db[1] = {GL_COLOR_ATTACHMENT0};
        glDrawBuffers(2,db);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) NSEngine::error("error creating surface");
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER,0);
        int t = AddTexture(tex.id,x,y);
        surfaces.push_back({t,fb,ratio});
        return r;
    }

    void TextureManager::SetSurfaceTarget(size_t i, int x1, int y1, int x2, int y2)
    {
        if (i >= surfaces.size()) return;
        glBindTexture(GL_TEXTURE_2D,0);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER,surfaces[i].framebuffer);
        if (x1 == -1)
        {
            x1 = 0;y1 = 0;
            x2 = textures[surfaces[i].surf].width;y2 = textures[surfaces[i].surf].height;
        }
        glViewport(x1,y1,x2,y2);
        setCamBoundaries(x2-x1,y2-y1);
    }

    GLuint TextureManager::GetSurfaceTexID(int i)
    {
        return surfaces[i].surf;
    }

}



