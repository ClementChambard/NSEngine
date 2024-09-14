#include "Terrain.h"
#include "Loader3D.h"
#include <math/math.hpp>
#include <vendor/stb_image.h>

namespace ns::n3d {

    RawModel* Terrain::generate_terrain(cstr heightmap_image) {
        bytes img; i32 w, h, nr_channels;

        img = stbi_load(heightmap_image, &w, &h, &nr_channels, 4);

        VERTEX_COUNT = h;

        i32 count = VERTEX_COUNT * VERTEX_COUNT;
        heights = new f32[VERTEX_COUNT*VERTEX_COUNT];
        f32* positions = new f32[count * 3];
        f32* normals = new f32[count * 3];
        f32* texCoords = new f32[count * 2];
        u32* indices = new u32[6*(VERTEX_COUNT-1)*(VERTEX_COUNT-1)];
        u32 vertexPointer = 0;

        for (i32 i = 0; i < VERTEX_COUNT; i++)
            for (i32 j = 0; j < VERTEX_COUNT; j++) {
                f32 heighthere = get_height(j, i, img, w, h, nr_channels);
                heights[j*VERTEX_COUNT+i] = heighthere;
                positions[vertexPointer * 3    ] = -(f32)j/((f32)VERTEX_COUNT-1) * SIZE;
                positions[vertexPointer * 3 + 1] = heighthere;
                positions[vertexPointer * 3 + 2] = -(f32)i/((f32)VERTEX_COUNT-1) * SIZE;
                vec3 normal = calc_normal(j, i, img, w, h, nr_channels);
                normals[vertexPointer * 3    ] = normal.x;
                normals[vertexPointer * 3 + 1] = normal.y;
                normals[vertexPointer * 3 + 2] = normal.z;
                texCoords[vertexPointer * 2    ] = (f32)j/((f32)VERTEX_COUNT-1);
                texCoords[vertexPointer * 2 + 1] = (f32)i/((f32)VERTEX_COUNT-1);
                vertexPointer++;
            }

        u32 pointer = 0;
        for (i32 gz = 0; gz < VERTEX_COUNT-1; gz++)
            for (i32 gx = 0; gx < VERTEX_COUNT-1; gx++) {
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
        stbi_image_free(img);

        return Loader3D::load_to_vao(positions, count*3, texCoords, count*2, normals, count*3, indices, 6*(VERTEX_COUNT-1)*(VERTEX_COUNT-1));
    }

    f32 Terrain::get_height(i32 x, i32 y, bytes buff, i32 w, i32 h, i32 nr_channels)
    {
        if (x <= 0 || x >= h-1 || z <= 0 || z >= h-1) return 0;
        u8 r = buff[(x*w+y)*nr_channels];
        u8 g = buff[(x*w+y)*nr_channels+1];
        u8 b = buff[(x*w+y)*nr_channels+2];
        f32 height = (r+g+b)* MAX_HEIGHT / MAX_PIXEL_COLOR;
        return height;
    }

    vec3 Terrain::calc_normal(i32 x, i32 y, bytes buff, i32 w, i32 h, i32 nr_channels)
    {
        f32 heightl = get_height(x+1, y, buff, w, h, nr_channels);
        f32 heightr = get_height(x-1, y, buff, w, h, nr_channels);
        f32 heightd = get_height(x, y+1, buff, w, h, nr_channels);
        f32 heightu = get_height(x, y-1, buff, w, h, nr_channels);
        return vec3(heightl-heightr, 2, heightd-heightu).normalized();
    }

    f32 Terrain::get_height_at(f32 x, f32 z)
    {
        f32 xx = x - this->x;
        f32 zz = z - this->z;
        f32 grid_square_size = SIZE / ((f32)VERTEX_COUNT-1);
        i32 grid_x = (i32) ns::floor(xx/grid_square_size);
        i32 grid_z = (i32) ns::floor(zz/grid_square_size);
        if (grid_x >= VERTEX_COUNT-1 || grid_x < 0 || grid_z >= VERTEX_COUNT-1 || grid_z < 0) return 0;
        f32 xcoords = ns::mod(xx,grid_square_size)/grid_square_size;
        f32 zcoords = ns::mod(zz,grid_square_size)/grid_square_size;
        f32 anwser;
        if (xcoords <= 1-zcoords)
            anwser = math::barycentric({0,heights[grid_x*VERTEX_COUNT+grid_z],0},
                                       {1,heights[(grid_x+1)*VERTEX_COUNT+grid_z],0},
                                       {0,heights[grid_x*VERTEX_COUNT+grid_z+1],1},
                                       {});
        else
            anwser = math::barycentric({1,heights[(grid_x+1)*VERTEX_COUNT+grid_z],0},
                                       {1,heights[(grid_x+1)*VERTEX_COUNT+grid_z+1],1},
                                       {0,heights[grid_x*VERTEX_COUNT+grid_z+1],1},
                                       {});
        return anwser;
    }

}
