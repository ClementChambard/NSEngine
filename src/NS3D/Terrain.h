#ifndef TERRAIN_INCLUDED_H
#define TERRAIN_INCLUDED_H

#include "TexturedModel.h"
#include "TerrainTexture.h"
#include <glm/glm.hpp>
#include <string>

namespace NS3D {

    class Terrain {
        public:
            Terrain(int gridX, int gridZ, TerrainTexture* tex, GLuint BMTexID, std::string heightmapImage) :
                texture(tex), x(gridX * SIZE), z(gridZ * SIZE),
                model(generateTerrain(heightmapImage)), blendMapTexID(BMTexID) {}
            ~Terrain() { delete model; delete[] heights; }

            float getX() const { return x; }
            float getZ() const { return z; }
            RawModel* getModel() const { return model; }
            TerrainTexture* getTexture() const { return texture; }
            GLuint getBlendMap() const { return blendMapTexID; }
            float getHeightAt(float x, float z);

        private:
            static const float SIZE;
            static const float MAX_HEIGHT;
            static const int MAX_PIXEL_COLOR;
            int VERTEX_COUNT;
            float* heights;

            RawModel* generateTerrain(std::string heightmapImage);
            float getHeight(int x, int y, unsigned char* buff, int w, int h, int nr_channels);
            glm::vec3 calcNormal(int x, int y, unsigned char* buff, int w, int h, int nr_channels);

            TerrainTexture* texture;
            float x, z;
            RawModel* model;
            GLuint blendMapTexID;
    };

}

#endif
