#ifndef TERRAIN_HEADER_INCLUDED
#define TERRAIN_HEADER_INCLUDED

#include "RawModel.h"
#include "TerrainTexture.h"
#include <defines.h>

namespace ns::n3d {

class Terrain {
    public:
        Terrain(i32 grid_x, i32 grid_z, TerrainTexture* tex, Texture* bm_tex, cstr heightmap_image) :
            texture(tex), x(grid_x * SIZE), z(grid_z * SIZE),
            model(generate_terrain(heightmap_image)), blendmap_tex(bm_tex) {}
        ~Terrain() { delete model; delete[] heights; }

        f32 get_x() const { return x; }
        f32 get_z() const { return z; }
        RawModel* get_model() const { return model; }
        TerrainTexture* get_texture() const { return texture; }
        Texture* get_blendmap() const { return blendmap_tex; }
        f32 get_height_at(f32 x, f32 z);

    private:
        static constexpr f32 SIZE = 800;
        static constexpr f32 MAX_HEIGHT = 65;//120;
        static constexpr i32 MAX_PIXEL_COLOR = 256+256+256;
        i32 VERTEX_COUNT;
        f32* heights;

        RawModel* generate_terrain(cstr heightmap_image);
        f32 get_height(i32 x, i32 y, bytes buff, i32 w, i32 h, i32 nr_channels);
        vec3 calc_normal(i32 x, i32 y, bytes buff, i32 w, i32 h, i32 nr_channels);

        TerrainTexture* texture;
        f32 x, z;
        RawModel* model;
        Texture* blendmap_tex;
};

}

#endif // TERRAIN_HEADER_INCLUDED
