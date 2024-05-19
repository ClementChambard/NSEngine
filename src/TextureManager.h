#ifndef TEXTUREMANAGER
#define TEXTUREMANAGER

#include <vector>
#include <glm/glm.hpp>
#include "./Texture.hpp"
#include "./defines.h"

namespace ns {

enum blendmode {bm_normal = 0, bm_add, bm_substract};

class TextureManager {
    public:
        static usize RegisterTexture(cstr name);
        static void UseTexture(usize i);
        static void SetBlendmode(i32 i);
        static void ResetTexture();
        static usize AddTexture(GLuint texID, i32 w, i32 h);
        static usize AddTexture(Texture&& t);
        static void GetTextureSize(usize i, i32 &w, i32 &h);
        static GLuint GetTextureID(usize i);
        static Texture* GetTexture(usize i);
        static Texture* GetTextureByOpengl(GLuint texID);
        static usize GetTextureNSIDByOpengl(GLuint texID);
        static glm::vec2 GetUVAt(usize i, i32 x, i32 y);
        static usize CreateSurface(i32 x, i32 y, f32 ratio = 1);
        static void SetSurfaceTarget(usize i, i32 x1 = -1, i32 y1 = -1, i32 x2 = -1, i32 y2 = -1);
        static GLuint GetSurfaceTexID(usize i);
    private:
        static std::vector<Texture> textures;
        static usize currentTexture;
};

}

#endif
