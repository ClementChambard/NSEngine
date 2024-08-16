#ifndef DRAWBATCH_HEADER_INCLUDED
#define DRAWBATCH_HEADER_INCLUDED

#include <vector>
#include "./math/types/types.hpp"
#include "./defines.h"
#include "./color.h"
#include "Texture.hpp"
#include "./SpriteBatch.h"

namespace ns {

struct RenderQuad { Vertex data[4]; };

class DrawBatch {
    public:
        DrawBatch();
        ~DrawBatch();

        void begin();
        void end(bool staticdraw = false);
        void clear();

        void draw(Vertex tl, Vertex tr, Vertex br, Vertex bl);

        void render_batch();
        void submit(bool clean_buffer = true);

        bool is_empty();

        std::size_t size() const { return m_quads.size(); }

    private:
        u32 m_vbo; // TODO: gl specific data (or create Buffer class)
        u32 m_vao;
        std::vector<RenderQuad> m_quads;
        void create_batch(bool staticdraw);
        void sort_quads(bool sort = true);
};

} // namespace ns

#endif // DRAWBATCH_HEADER_INCLUDED
