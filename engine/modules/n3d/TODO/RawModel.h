#ifndef RAWMODEL_HEADER_INCLUDED
#define RAWMODEL_HEADER_INCLUDED

#include <defines.h>

namespace ns::n3d {

class RawModel {
    public:
        RawModel(u32 vao_id, u32 vertex_count) :
            vao_id(vao_id), vertex_count(vertex_count) {}

        u32 get_vao_id() const { return vao_id; }
        u32 get_vertex_count() const { return vertex_count; }

    private:
        u32 vao_id;
        u32 vertex_count;
};

}

#endif // RAWMODEL_HEADER_INCLUDED
