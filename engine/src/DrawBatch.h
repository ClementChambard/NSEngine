#ifndef DRAWBATCH_HEADER_INCLUDED
#define DRAWBATCH_HEADER_INCLUDED

#include <vector>
#include "./math/types/vec/vec2.hpp"
#include "./math/types/vec/vec3.hpp"
#include "./defines.h"
#include "./color.h"

namespace ns {

struct Vertex {
    vec3 position = vec3(0.f, 0.f, 0.f);
    Color color = Color(255, 255, 255, 255);
    vec2 uv = vec2(0.f, 0.f);
};

/**
 * Class abstracting vertex/index buffers.
 * Can be used as a static 3d model or as a batch reused multiple times per frames.
 */
class DrawBatch {
public:
    // TODO: Maybe use a generic to have a DrawBatch<u16> for example
    using Index = u32;

    DrawBatch();
    ~DrawBatch();

    /**
     * Clears the existing data to start new batch content
     */
    void begin();

    /**
     * Creates data buffers from batch content
     */
    void end(bool staticdraw = false);

    /**
     * Clears all batch content
     */
    void clear();

    /**
     * Adds a vertex to the vertex buffer and returns its index.
     */
    Index add_vertex(Vertex v);

    /**
     * Adds 3 vertices forming a triangle and returns the first index.
     */
    Index draw_tri(Vertex v1, Vertex v2, Vertex v3);

    /**
     * Adds a triangle from 3 indices.
     */
    void draw_tri(Index v1, Index v2, Index v3);

    /**
     * Adds 4 vertices forming a quad and returns the first index.
     */
    Index draw_quad(Vertex tl, Vertex tr, Vertex br, Vertex bl);

    /**
     * Adds a quad from 4 indices.
     */
    void draw_quad(Index tl, Index tr, Index br, Index bl);

    /**
     * Adds a triangle from the two last vertex and a new one with index returned.
     */
    Index draw_tri_strip(Vertex v);

    /**
     * Adds a triangle from the two last vertex and a new one from its index.
     */
    void draw_tri_strip(Index v);

    /**
     * Adds a quad from the two last vertex and two new ones, first index returned.
     */
    Index draw_quad_strip(Vertex v1, Vertex v2);

    /**
     * Adds a quad from the two last vertex and two new ones from their indices.
     */
    void draw_quad_strip(Index v1, Index v2);

    /**
     * Gets the index that would be returned if adding a new vertex.
     */
    Index get_next_vid() const;

    /**
     * Draws the buffers (created by a call to 'end')
     */
    void render_batch() const;

    /**
     * Creates data buffers from batch content, then draw the buffers.
     * Optionnally clear batch content afterwards
     */
    void submit(bool clean_buffer = true);

    /**
     * Checks if the batch has no content
     */
    bool is_empty() const;

    usize size() const { return m_indices.size(); }

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

#endif // DRAWBATCH_HEADER_INCLUDED
