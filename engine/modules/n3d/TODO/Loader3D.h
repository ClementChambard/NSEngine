#ifndef LOADER3D_HEADER_INCLUDED
#define LOADER3D_HEADER_INCLUDED

#include "RawModel.h"
#include "ModelData.h"
#include <vector>
#include <defines.h>

namespace ns::n3d {

class Loader3D {
    public:
        static RawModel* load_to_vao(f32* positions,      u32 positions_l,
                                     f32* texture_coords, u32 texture_coords_l,
                                     f32* normals,        u32 normals_l,
                                     u32* indices,        u32 indices_l);
        static RawModel* load_to_vao(f32* positions,      u32 positions_l,
                                     u32  dimensions);
        static RawModel* load_to_vao(ModelData* md);

        static u32 create_vao();

        //Right Left Top Bottom Back Front
        static u32 load_cube_map(const std::vector<cstr> images);

        static void cleanup();

    private:
        static void store_data_in_attribute_list(i32 attribute_number, u32 dimensions, f32* data, u32 data_l);
        static void unbind_vao();
        static void bind_indices_buffer(u32* indices, u32 indices_l);

        static std::vector<u32> created_vaos;
        static std::vector<u32> created_vbos;
        static std::vector<u32> created_texs;
};

}

#endif // LOADER3D_HEADER_INCLUDED
