#include "Terrain.h"
#include "Loader3D.h"
#include "../ImageLoader.h"
#include "../math/math.h"

namespace NS3D {

    RawModel* Terrain::generateTerrain(cstr heightmapImage)
    {
        bytes img; i32 w, h, nr_channels;
        img = ns::ImageLoader::imageToBuffer(heightmapImage, w, h, nr_channels);

        VERTEX_COUNT = h;

        i32 count = VERTEX_COUNT * VERTEX_COUNT;
        heights = new f32[VERTEX_COUNT*VERTEX_COUNT];
        f32* positions = new f32[count * 3];
        f32* normals = new f32[count * 3];
        f32* texCoords = new f32[count * 2];
        u32* indices = new u32[6*(VERTEX_COUNT-1)*(VERTEX_COUNT-1)];
        u32 vertexPointer = 0;

        for (i32 i = 0; i < VERTEX_COUNT; i++)
            for (i32 j = 0; j < VERTEX_COUNT; j++)
            {
                f32 heighthere = getHeight(j, i, img, w, h, nr_channels);
                heights[j*VERTEX_COUNT+i] = heighthere;
                positions[vertexPointer*3    ] = -(f32)j/((f32)VERTEX_COUNT-1) * SIZE;
                positions[vertexPointer*3 + 1] = heighthere;
                positions[vertexPointer*3 + 2] = -(f32)i/((f32)VERTEX_COUNT-1) * SIZE;
                glm::vec3 normal = calcNormal(j, i, img, w, h, nr_channels);
                normals[vertexPointer*3    ] = normal.x;
                normals[vertexPointer*3 + 1] = normal.y;
                normals[vertexPointer*3 + 2] = normal.z;
                texCoords[vertexPointer*2    ] = (f32)j/((f32)VERTEX_COUNT-1);
                texCoords[vertexPointer*2 + 1] = (f32)i/((f32)VERTEX_COUNT-1);
                vertexPointer++;
            }

        u32 pointer = 0;
        for (i32 gz = 0; gz < VERTEX_COUNT-1; gz++)
            for (i32 gx = 0; gx < VERTEX_COUNT-1; gx++)
            {
                u32 tl = (gz * VERTEX_COUNT) + gx;
                u32 tr = tl + 1;
                u32 bl = ((gz + 1) * VERTEX_COUNT) + gx;
                u32 br = bl + 1;
                indices[pointer++] = tl;
                indices[pointer++] = bl;
                indices[pointer++] = tr;
                indices[pointer++] = tr;
                indices[pointer++] = bl;
                indices[pointer++] = br;
            }

        return Loader3D::loadToVAO(positions, count*3, texCoords, count*2, normals, count*3, indices, 6*(VERTEX_COUNT-1)*(VERTEX_COUNT-1));
    }

    f32 Terrain::getHeight(i32 x, i32 y, bytes buff, i32 w, i32 h, i32 nr_channels)
    {
        if (x <= 0 || x >= h-1 || z <= 0 || z >= h-1) return 0;
        u8 r = buff[(x*w+y)*nr_channels];
        u8 g = buff[(x*w+y)*nr_channels+1];
        u8 b = buff[(x*w+y)*nr_channels+2];
        f32 height = (r+g+b)* MAX_HEIGHT / MAX_PIXEL_COLOR;
        return height;
    }

    glm::vec3 Terrain::calcNormal(i32 x, i32 y, bytes buff, i32 w, i32 h, i32 nr_channels)
    {
        f32 heightl = getHeight(x+1, y, buff, w, h, nr_channels);
        f32 heightr = getHeight(x-1, y, buff, w, h, nr_channels);
        f32 heightd = getHeight(x, y+1, buff, w, h, nr_channels);
        f32 heightu = getHeight(x, y-1, buff, w, h, nr_channels);
        return glm::normalize(glm::vec3(heightl-heightr, 2, heightd-heightu));
    }

    f32 Terrain::getHeightAt(f32 x, f32 z)
    {
        f32 xx = x - this->x;
        f32 zz = z - this->z;
        f32 gridSquareSize = SIZE / ((f32)VERTEX_COUNT-1);
        i32 gridX = (i32) floor(xx/gridSquareSize);
        i32 gridZ = (i32) floor(zz/gridSquareSize);
        if (gridX >= VERTEX_COUNT-1 || gridX < 0 || gridZ >= VERTEX_COUNT-1 || gridZ < 0) return 0;
        f32 xcoords = math::mod(xx,gridSquareSize)/gridSquareSize;
        f32 zcoords = math::mod(zz,gridSquareSize)/gridSquareSize;
        f32 anwser;
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
