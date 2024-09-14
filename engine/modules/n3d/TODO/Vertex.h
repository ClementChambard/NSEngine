#ifndef VERTEX_HEADER_INCLUDED
#define VERTEX_HEADER_INCLUDED

#include <defines.h>
#include <math/types/types.hpp>

#define NO_INDEX (u32)(-1)

namespace ns::n3d {

class Vertex {
    public:
        Vertex(i32 id, const vec3& pos) :
            index(id), position(pos), length(pos.length()) {}
        ~Vertex();

        u32 get_index() const { return index; }
        f32 get_length() const { return length; }
        bool is_set() const { return texture_index != NO_INDEX && normal_index != NO_INDEX; }
        bool has_same_texture_and_normal(u32 tex, u32 nor) const { return texture_index == tex && normal_index == nor; }

        void set_texture_index(u32 id) { texture_index = id; }
        void set_normal_index(u32 id) { normal_index = id; }
        void set_duplicate(Vertex* v) { duplicate_vertex = v; }

        vec3 get_position() const { return position; }
        u32 get_texture_index() const { return texture_index; }
        u32 get_normal_index() const { return normal_index; }
        Vertex* get_duplicate() const { return duplicate_vertex; }

    private:
        u32 index;
        vec3 position;
        f32 length;
        u32 texture_index = NO_INDEX;
        u32 normal_index = NO_INDEX;
        Vertex* duplicate_vertex = nullptr;
};

}

#endif // VERTEX_HEADER_INCLUDED
