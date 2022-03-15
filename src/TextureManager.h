#ifndef TEXTUREMANAGER
#define TEXTUREMANAGER

#include "ImageLoader.h"
#include "vertex.h"
#include <vector>

namespace NSEngine {

    enum blendmode {bm_normal = 0, bm_add, bm_substract};

    class TextureManager {
        public:
            static int RegisterTexture(const char* name, const char* name2 = "", const char* name3 = "", const char* name4 = "");
            static void UseTexture(int i);
            static void SetBlendmode(int i);
            static void ResetTexture();
            static int AddTexture(GLuint texID, int w, int h);
            static void GetTextureSize(int i, int &w, int &h);
            static GLuint GetTextureID(int i);
            static GLTexture* GetTexture(int i);
            static UV GetUVAt(int i, int x, int y);
            static int CreateSurface(int x, int y, float ratio = 1);
            static void SetSurfaceTarget(int i, int x1 = -1, int y1 = -1, int x2 = -1, int y2 = -1);
            static GLuint GetSurfaceTexID(int i);
        private:
            static std::vector<GLTexture> textures;
            static std::vector<GLSurface> surfaces;
            static int currentTexture;
    };

}

#endif
