#ifndef MODELDATA_HEADER_INCLUDED
#define MODELDATA_HEADER_INCLUDED

#include <defines.h>

namespace ns::n3d {

class ModelData {
    public:
        ModelData(f32* v, f32* t, f32* n, u32* i, f32 fp, u32 nv, u32 ni) :
            vertices(v), texture_coords(t), normals(n),
            indices(i), furthest_point(fp), num_vertices(nv), num_indices(ni) {}
        ~ModelData() { delete vertices; delete normals; delete texture_coords; delete indices; }

        f32* get_vertices() const { return vertices; }
        f32* get_texture_coords() const { return texture_coords; }
        f32* get_normals() const { return normals; }
        u32* get_indices() const { return indices; }
        f32 get_furthest_point() const { return furthest_point; }
        u32 get_num_vertices() const { return num_vertices; }
        u32 get_num_indices() const { return num_indices; }

    private:
        f32* vertices;
        f32* texture_coords;
        f32* normals;
        u32* indices;
        f32 furthest_point;
        u32 num_vertices;
        u32 num_indices;
};

}

#endif // MODELDATA_HEADER_INCLUDED
