#ifndef TERRAINRENDERER_HEADER_INCLUDED
#define TERRAINRENDERER_HEADER_INCLUDED

#include <vector>
#include "TerrainShader.h"
#include "Terrain.h"

namespace ns::n3d {

class TerrainRenderer {
    public:
        TerrainRenderer(TerrainShader* shad);
        ~TerrainRenderer() { delete shader; }

        void render(const std::vector<const Terrain*>& terrains, const std::vector<const Light*>& light);

    private:
        TerrainShader* shader;

        void prepare_terrain(const Terrain* terrain);
        void bind_textures(const Terrain* terrain);
        void unbind_terrain();
        void load_model_matrix(const Terrain* terrain);
    friend class Renderer3D;
};

}

#endif // TERRAINRENDERER_HEADER_INCLUDED
