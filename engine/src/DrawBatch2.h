#ifndef DRAWBATCH2_HEADER_INCLUDED
#define DRAWBATCH2_HEADER_INCLUDED

#include <vector>
#include "./math/types/types.hpp"
#include "./defines.h"
#include "./color.h"
#include "Texture.hpp"
#include "./SpriteBatch.h"

namespace ns {

class DrawBatch2 {
    using Index = u32;
    public:
        DrawBatch2();
        ~DrawBatch2();

        void begin();
        void end(bool staticdraw = false);
        void clear();

        u32 add_vertex(Vertex v);
        u32 draw_tri(Vertex v1, Vertex v2, Vertex v3);
        void draw_tri(u32 v1, u32 v2, u32 v3);
        u32 draw_quad(Vertex tl, Vertex tr, Vertex br, Vertex bl);
        void draw_quad(u32 tl, u32 tr, u32 br, u32 bl);
        u32 draw_tri_strip(Vertex v);
        void draw_tri_strip(u32 v);
        u32 draw_quad_strip(Vertex v1, Vertex v2);
        void draw_quad_strip(u32 v1, u32 v2);

        // old interface
        void draw(Vertex tl, Vertex tr, Vertex br, Vertex bl) { draw_quad(tl, tr, br, bl); }

        void render_batch();
        void submit(bool clean_buffer = true);

        bool is_empty();

        std::size_t size() const { return m_indices.size(); }

    private:
        u32 m_vbo; // TODO: gl specific data (or create Buffer class)
        u32 m_ibo;
        u32 m_vao;
        std::vector<Vertex> m_vertices;
        std::vector<Index> m_indices;
        void create_batch(bool staticdraw = false);
        void sort_quads(bool sort = true);
};

} // namespace ns

#endif // DRAWBATCH2_HEADER_INCLUDED
