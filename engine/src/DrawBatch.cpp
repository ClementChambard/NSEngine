#include "DrawBatch.h"

#include <GL/glew.h>

namespace ns {

DrawBatch::DrawBatch() {
    glCreateVertexArrays(1, &m_vao);
    glCreateBuffers(1, &m_vbo);
    glCreateBuffers(1, &m_ibo);

    glVertexArrayElementBuffer(m_vao, m_ibo);
    glVertexArrayVertexBuffer(m_vao, 0, m_vbo, 0, sizeof(Vertex));

    glEnableVertexArrayAttrib(m_vao, 0);
    glEnableVertexArrayAttrib(m_vao, 1);
    glEnableVertexArrayAttrib(m_vao, 2);

    glVertexArrayAttribBinding(m_vao, 0, 0);
    glVertexArrayAttribBinding(m_vao, 1, 0);
    glVertexArrayAttribBinding(m_vao, 2, 0);

    glVertexArrayAttribFormat(m_vao, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, position));
    glVertexArrayAttribFormat(m_vao, 1, 4, GL_UNSIGNED_BYTE, GL_TRUE, offsetof(Vertex, color));
    glVertexArrayAttribFormat(m_vao, 2, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, uv));
}

DrawBatch::~DrawBatch() {
    glBindVertexArray(0);

    if (m_vao != 0) glDeleteVertexArrays(1, &m_vao);
    if (m_vbo != 0) glDeleteBuffers(1, &m_vbo);
    if (m_ibo != 0) glDeleteBuffers(1, &m_ibo);
}

void DrawBatch::begin() { clear(); }

void DrawBatch::end(bool staticdraw) { create_batch(staticdraw); }

void DrawBatch::clear() {
    m_vertices.clear();
    m_indices.clear();
}

bool DrawBatch::is_empty() const {
    return m_indices.size() == 0;
}

u32 DrawBatch::add_vertex(Vertex v) {
    u32 idx = m_vertices.size();
    m_vertices.push_back(v);
    return idx;
}

u32 DrawBatch::draw_tri(Vertex v1, Vertex v2, Vertex v3) {
    u32 fst_idx = add_vertex(v1);
    add_vertex(v2);
    add_vertex(v3);
    draw_tri(fst_idx, fst_idx + 1, fst_idx + 2);
    return fst_idx;
}

u32 DrawBatch::draw_quad(Vertex tl, Vertex tr, Vertex br, Vertex bl) {
    u32 fst_idx = add_vertex(tl);
    add_vertex(tr);
    add_vertex(br);
    add_vertex(bl);
    draw_quad(fst_idx, fst_idx + 1, fst_idx + 2, fst_idx + 3);
    return fst_idx;
}

u32 DrawBatch::draw_tri_strip(Vertex v) {
    u32 idx = add_vertex(v);
    draw_tri_strip(idx);
    return idx;
}

u32 DrawBatch::draw_quad_strip(Vertex v1, Vertex v2) {
    u32 fst_idx = add_vertex(v1);
    add_vertex(v2);
    draw_quad_strip(fst_idx, fst_idx + 1);
    return fst_idx;
}

void DrawBatch::draw_tri(u32 v1, u32 v2, u32 v3) {
    m_indices.push_back(v1);
    m_indices.push_back(v2);
    m_indices.push_back(v3);
}

void DrawBatch::draw_quad(u32 tl, u32 tr, u32 br, u32 bl) {
    draw_tri(tl, tr, br);
    draw_tri(tl, br, bl);
}

void DrawBatch::draw_tri_strip(u32 v) {
    draw_tri(v - 2, v - 1, v);
}

void DrawBatch::draw_quad_strip(u32 v1, u32 v2) {
    draw_quad(v1 - 2, v2 - 2, v1, v2);
}

u32 DrawBatch::get_next_vid() const {
    return m_vertices.size();
}

void DrawBatch::render_batch() const {
    glBindVertexArray(m_vao);

    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
}

void DrawBatch::submit(bool clear_glyphs) {
    create_batch();

    render_batch();

    if (clear_glyphs) clear();
}

void DrawBatch::create_batch(bool staticdraw)
{
    if (m_vertices.empty() || m_indices.empty()) return;

    auto dt = staticdraw ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW;

    glNamedBufferData(m_vbo, m_vertices.size() * sizeof(Vertex), m_vertices.data(), dt);
    glNamedBufferData(m_ibo, m_indices.size() * sizeof(Index), m_indices.data(), dt);
}

}
