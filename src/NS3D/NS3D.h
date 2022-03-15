#ifndef NS3D_INCLUDED_H
#define NS3D_INCLUDED_H

#include "Renderer3D.h"
#include "OBJLoader.h"
#include "OBJFileLoader.h"
#include "Loader3D.h"
#include "WaterFrameBuffers.h"
#include "TexManager.h"

namespace NS3D {

inline void cleanup()
{
    TexManager::cleanup();
    Loader3D::cleanUp();
}

inline GLuint Tex(std::string tex) { return TexManager::addGLTexture(tex); }
inline TerrainTexture* TTex(GLuint t1, GLuint t2, GLuint t3, GLuint t4) { return TexManager::genTerrainTexture(t1,t2,t3,t4); }
inline ModelTexture* MTex(GLuint t) { return TexManager::genModelTexture(t); }

}

#endif //NS3D_INCLUDED_H
