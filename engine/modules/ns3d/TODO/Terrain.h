#ifndef TERRAIN_INCLUDED_H
#define TERRAIN_INCLUDED_H

#include "RawModel.h"
#include "TerrainTexture.h"
#include <glm/glm.hpp>
#include "../defines.h"

namespace NS3D {

class Terrain {
    public:
        Terrain(i32 gridX, i32 gridZ, TerrainTexture* tex, GLuint BMTexID, cstr heightmapImage) :
            texture(tex), x(gridX * SIZE), z(gridZ * SIZE),
            model(generateTerrain(heightmapImage)), blendMapTexID(BMTexID) {}
        ~Terrain() { delete model; delete[] heights; }

        f32 getX() const { return x; }
        f32 getZ() const { return z; }
        RawModel* getModel() const { return model; }
        TerrainTexture* getTexture() const { return texture; }
        GLuint getBlendMap() const { return blendMapTexID; }
        f32 getHeightAt(f32 x, f32 z);

    private:
        static constexpr f32 SIZE = 800;
        static constexpr f32 MAX_HEIGHT = 65;//120;
        static constexpr i32 MAX_PIXEL_COLOR = 256+256+256;
        i32 VERTEX_COUNT;
        f32* heights;

        RawModel* generateTerrain(cstr heightmapImage);
        f32 getHeight(i32 x, i32 y, bytes buff, i32 w, i32 h, i32 nr_channels);
        glm::vec3 calcNormal(i32 x, i32 y, bytes buff, i32 w, i32 h, i32 nr_channels);

        TerrainTexture* texture;
        f32 x, z;
        RawModel* model;
        GLuint blendMapTexID;
};

}

#endif
