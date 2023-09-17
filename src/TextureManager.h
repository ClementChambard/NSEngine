#ifndef TEXTUREMANAGER
#define TEXTUREMANAGER

#include "ImageLoader.h"
#include <vector>
#include <glm/glm.hpp>
#include "./Texture.hpp"

namespace NSEngine {

    enum blendmode {bm_normal = 0, bm_add, bm_substract};

    class TextureManager {
        public:
            static size_t RegisterTexture(const char* name);
            static void UseTexture(size_t i);
            static void SetBlendmode(int i);
            static void ResetTexture();
            static size_t AddTexture(GLuint texID, int w, int h);
            static size_t AddTexture(Texture&& t);
            static void GetTextureSize(size_t i, int &w, int &h);
            static GLuint GetTextureID(size_t i);
            static Texture* GetTexture(size_t i);
            static Texture* GetTextureByOpengl(GLuint texID);
            static size_t GetTextureNSIDByOpengl(GLuint texID);
            static glm::vec2 GetUVAt(size_t i, int x, int y);
            static size_t CreateSurface(int x, int y, float ratio = 1);
            static void SetSurfaceTarget(size_t i, int x1 = -1, int y1 = -1, int x2 = -1, int y2 = -1);
            static GLuint GetSurfaceTexID(size_t i);
        private:
            static std::vector<Texture> textures;
            static size_t currentTexture;
    };

}

#endif
