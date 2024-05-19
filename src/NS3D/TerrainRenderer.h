#ifndef TERRAINRENDERER_INCLUDED_H
#define TERRAINRENDERER_INCLUDED_H

#include <vector>
#include "TerrainShader.h"
#include "Terrain.h"

namespace NS3D {

class TerrainRenderer {
    public:
        TerrainRenderer(TerrainShader* shad);
        ~TerrainRenderer() { delete shader; }

        void render(const std::vector<const Terrain*>& terrains, const std::vector<const Light*>& light);

    private:
        TerrainShader* shader;

        void prepareTerrain(const Terrain* terrain);
        void bindTextures(const Terrain* terrain);
        void unbindTerrain();
        void loadModelMatrix(const Terrain* terrain);
    friend class Renderer3D;
};

}

#endif
