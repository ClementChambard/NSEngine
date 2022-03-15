#include "Terrain.h"
#include "Loader3D.h"
#include "../ImageLoader.h"
#include "../math/math.h"

namespace NS3D {

    const float Terrain::SIZE = 800;
    const float Terrain::MAX_HEIGHT = 65;//120;
    const int Terrain::MAX_PIXEL_COLOR = 256+256+256;

    RawModel* Terrain::generateTerrain(std::string heightmapImage)
    {
        unsigned char* img; int w, h, nr_channels;
        img = NSEngine::ImageLoader::imageToBuffer(heightmapImage.c_str(), w, h, nr_channels);

        VERTEX_COUNT = h;

        int count = VERTEX_COUNT * VERTEX_COUNT;
        heights = new float[VERTEX_COUNT*VERTEX_COUNT];
        float* positions = new float[count * 3];
        float* normals = new float[count * 3];
        float* texCoords = new float[count * 2];
        unsigned int* indices = new unsigned int[6*(VERTEX_COUNT-1)*(VERTEX_COUNT-1)];
        unsigned int vertexPointer = 0;

        for (int i = 0; i < VERTEX_COUNT; i++)
            for (int j = 0; j < VERTEX_COUNT; j++)
            {
                float heighthere = getHeight(j, i, img, w, h, nr_channels);
                heights[j*VERTEX_COUNT+i] = heighthere;
                positions[vertexPointer*3    ] = -(float)j/((float)VERTEX_COUNT-1) * SIZE;
                positions[vertexPointer*3 + 1] = heighthere;
                positions[vertexPointer*3 + 2] = -(float)i/((float)VERTEX_COUNT-1) * SIZE;
                glm::vec3 normal = calcNormal(j, i, img, w, h, nr_channels);
                normals[vertexPointer*3    ] = normal.x;
                normals[vertexPointer*3 + 1] = normal.y;
                normals[vertexPointer*3 + 2] = normal.z;
                texCoords[vertexPointer*2    ] = (float)j/((float)VERTEX_COUNT-1);
                texCoords[vertexPointer*2 + 1] = (float)i/((float)VERTEX_COUNT-1);
                vertexPointer++;
            }
        
        unsigned int pointer = 0;
        for (int gz = 0; gz < VERTEX_COUNT-1; gz++)
            for (int gx = 0; gx < VERTEX_COUNT-1; gx++)
            {
                unsigned int tl = (gz * VERTEX_COUNT) + gx;
                unsigned int tr = tl + 1;
                unsigned int bl = ((gz + 1) * VERTEX_COUNT) + gx;
                unsigned int br = bl + 1;
                indices[pointer++] = tl;
                indices[pointer++] = bl;
                indices[pointer++] = tr;
                indices[pointer++] = tr;
                indices[pointer++] = bl;
                indices[pointer++] = br;
            }


        return Loader3D::loadToVAO(positions, count*3, texCoords, count*2, normals, count*3, indices, 6*(VERTEX_COUNT-1)*(VERTEX_COUNT-1));
    }

    float Terrain::getHeight(int x, int y, unsigned char* buff, int w, int h, int nr_channels)
    {
        if (x < 0 || x >= h || z < 0 || z >= h) return 0;
        unsigned char r = buff[(x*w+y)*nr_channels];
        unsigned char g = buff[(x*w+y)*nr_channels+1];
        unsigned char b = buff[(x*w+y)*nr_channels+2];
        float height = (r+g+b)* MAX_HEIGHT / MAX_PIXEL_COLOR;
        return height;
    }

    glm::vec3 Terrain::calcNormal(int x, int y, unsigned char* buff, int w, int h, int nr_channels)
    {
        float heightl = getHeight(x+1, y, buff, w, h, nr_channels);
        float heightr = getHeight(x-1, y, buff, w, h, nr_channels);
        float heightd = getHeight(x, y+1, buff, w, h, nr_channels);
        float heightu = getHeight(x, y-1, buff, w, h, nr_channels);
        return glm::normalize(glm::vec3(heightl-heightr, 2, heightd-heightu));
    }

    float Terrain::getHeightAt(float x, float z)
    {
        float xx = x - this->x;   
        float zz = z - this->z;
        float gridSquareSize = SIZE / ((float)VERTEX_COUNT-1);
        int gridX = (int) floor(xx/gridSquareSize);
        int gridZ = (int) floor(zz/gridSquareSize);
        if (gridX >= VERTEX_COUNT-1 || gridX < 0 || gridZ >= VERTEX_COUNT-1 || gridZ < 0) return 0;
        float xcoords = math::mod(xx,gridSquareSize)/gridSquareSize;
        float zcoords = math::mod(zz,gridSquareSize)/gridSquareSize;
        float anwser;
        if (xcoords <= 1-zcoords)
            anwser = math::barycentric({0,heights[gridX*VERTEX_COUNT+gridZ],0},
                                       {1,heights[(gridX+1)*VERTEX_COUNT+gridZ],0},
                                       {0,heights[gridX*VERTEX_COUNT+gridZ+1],1},
                                       {});
        else
            anwser = math::barycentric({1,heights[(gridX+1)*VERTEX_COUNT+gridZ],0},
                                       {1,heights[(gridX+1)*VERTEX_COUNT+gridZ+1],1},
                                       {0,heights[gridX*VERTEX_COUNT+gridZ+1],1},
                                       {});
        return anwser;
    }

}
