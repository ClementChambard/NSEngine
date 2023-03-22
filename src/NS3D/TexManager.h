#ifndef TEXMANAGER_INCLUDED_H
#define TEXMANAGER_INCLUDED_H

#include <string>
#include <GL/glew.h>
#include <vector>
#include "TerrainTexture.h"
#include "ModelTexture.h"
#include "../ImageLoader.h"

namespace NS3D { 

class TexManager {
    public:
    
        static int addModelTexture(GLuint texID) { createdModelTex.push_back(new ModelTexture(texID)); return createdModelTex.size()-1; }
        static ModelTexture* genModelTexture(GLuint texID) { return getModelTexture(addModelTexture(texID)); }
        static int addTerrainTexture(GLuint texID1, GLuint texID2, GLuint texID3, GLuint texID4) { createdTerrainTex.push_back(new TerrainTexture(texID1, texID2, texID3, texID4)); return createdTerrainTex.size()-1; }
        static TerrainTexture* genTerrainTexture(GLuint texID1, GLuint texID2, GLuint texID3, GLuint texID4) { return getTerrainTexture(addTerrainTexture(texID1, texID2, texID3, texID4));}
        static GLuint addGLTexture(std::string texFile) { GLuint i = NSEngine::ImageLoader::loadTexture(texFile.c_str()).id; createdGLTex.push_back(i); return i; }
        static ModelTexture* getModelTexture(int i) { return createdModelTex[i]; }
        static TerrainTexture* getTerrainTexture(int i) { return createdTerrainTex[i]; }
        static void cleanup() { for (auto m : createdModelTex) delete m; for (auto t : createdTerrainTex) delete t; for (auto t : createdGLTex) glDeleteTextures(1, &t);}

    private:
        static std::vector<GLuint> createdGLTex;
        static std::vector<ModelTexture*> createdModelTex;
        static std::vector<TerrainTexture*> createdTerrainTex;
};

}

#endif
