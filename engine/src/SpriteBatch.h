#ifndef SPRITEBATCH_HEADER_INCLUDED
#define SPRITEBATCH_HEADER_INCLUDED

#include <vector>
#include "./math/types/types.hpp"
#include "./defines.h"
#include "./color.h"
#include "Texture.hpp"

namespace ns {

struct Vertex {
    Vertex(f32 x, f32 y, f32 z, u8 r, u8 g, u8 b, u8 a, f32 u, f32 v)
    {
        position = vec3(x, y, z);
        color = Color(r, g, b, a);
        uv = vec2(u, v);
    }
    Vertex(const vec3& p, const Color& c, const vec2& u)
    {
        position = p;
        color = c;
        uv = u;
    }
    Vertex(const vec3& p, u8 r, u8 g, u8 b, u8 a, f32 u, f32 v)
    {
        position = p;
        color = Color(r, g, b, a);
        uv = vec2(u, v);
    }
    Vertex(f32 x, f32 y, f32 z, const Color& c, f32 u, f32 v)
    {
        position = vec3(x, y, z);
        color = c;
        uv = vec2(u, v);
    }
    Vertex(f32 x, f32 y, f32 z, u8 r, u8 g, u8 b, u8 a, const vec2& u)
    {
        position = vec3(x, y, z);
        color = Color(r, g, b, a);
        uv = u;
    }
    Vertex(const vec3& p, const Color& c, f32 u, f32 v)
    {
        position = p;
        color = c;
        uv = vec2(u, v);
    }
    Vertex(const vec3& p, u8 r, u8 g, u8 b, u8 a, const vec2 u)
    {
        position = p;
        color = Color(r, g, b, a);
        uv = u;
    }
    Vertex(f32 x, f32 y, f32 z, const Color& c, const vec2& u)
    {
        position = vec3(x, y, z);
        color = c;
        uv = u;
    }
    Vertex() {}
    Vertex(const vec3& p)
    {
        position = p;
    }
    Vertex(const Color& c)
    {
        color = c;
    }
    Vertex(const vec2& u)
    {
        uv = u;
    }
    Vertex(const vec3& p, const Color& c)
    {
        position = p;
        color = c;
    }
    Vertex(const Color& c, const vec2& u)
    {
        color = c;
        uv = u;
    }
    Vertex(const vec3& p, const vec2& u)
    {
        uv = u;
        position = p;
    }
    vec3 position = vec3(0.f, 0.f, 0.f);
    Color color = Color(255, 255, 255, 255);
    vec2 uv = vec2(0.f, 0.f);
};

inline bool operator==(Color c1,Color c2) { return c1.r == c2.r && c1.g == c2.g && c1.b == c2.b && c1.a == c2.a; }
inline bool operator==(Vertex v1, Vertex v2) { return v1.position == v2.position && v1.color == v2.color && v1.uv == v2.uv; }

struct Glyph {
   Glyph(ns::Texture* tex, Vertex tl, Vertex tr, Vertex br, Vertex bl, i32 bm = 0) : texture(tex), blendmode(bm), top_left(tl), top_right(tr), bottom_right(br), bottom_left(bl) {};

   ns::Texture* texture;
   u8 blendmode = 0; // TODO: Create blendmode enum
   Vertex top_left;
   Vertex top_right;
   Vertex bottom_right;
   Vertex bottom_left;
};

struct RenderBatch {
    RenderBatch(u32 ofs, u32 nb, ns::Texture* tex, i32 bm) : offset(ofs), num_vertices(nb), texture(tex), blendmode(bm) {}
    u32 offset;
    u32 num_vertices;
    ns::Texture* texture;
    u8 blendmode;
};

class SpriteBatch {
    public:
        SpriteBatch();
        ~SpriteBatch();

        SpriteBatch* init();
        void dispose() {}

        void begin();
        void end(bool staticdraw = false);
        void clear();

        void draw(u32 texture, Vertex tl, Vertex tr, Vertex br, Vertex bl, i32 blendmode = 0);
        void draw(ns::Texture* texture, Vertex tl, Vertex tr, Vertex br, Vertex bl, i32 blendmode = 0);

        void render_batch();
        void submit(bool clean_buffer = true);

        bool is_empty();

        std::size_t size() const { return m_glyphs.size(); }

    private:
        u32 m_vbo; // TODO: gl specific data (or create Buffer class)
        u32 m_vao;
        std::vector<Glyph> m_glyphs;
        std::vector<RenderBatch> m_renderBatches;
        void create_render_batches(bool staticdraw);
        void bind_buffer();
        void sort_glyphs(bool sort = true);
};

} // namespace ns

#endif // SPRITEBATCH_HEADER_INCLUDED
