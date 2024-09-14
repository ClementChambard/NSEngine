#ifndef N3D_HEADER_INCLUDED
#define N3D_HEADER_INCLUDED

#include "Loader3D.h"
#include "TexManager.h"

namespace ns::n3d {

inline void cleanup()
{
    TexManager::cleanup();
    Loader3D::cleanup();
}

inline Texture* tex(cstr tex) { return TexManager::gen_gl_texture(tex); }
inline TerrainTexture* t_tex(Texture* t1, Texture* t2, Texture* t3, Texture* t4) { return TexManager::gen_terrain_texture(t1,t2,t3,t4); }
inline ModelTexture* m_tex(Texture* t) { return TexManager::gen_model_texture(t); }

}

#endif // N3D_HEADER_INCLUDED
