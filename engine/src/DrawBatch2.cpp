#include "DrawBatch2.h"

#include <GL/glew.h>

namespace ns {

DrawBatch2::DrawBatch2() {
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glGenBuffers(1, &m_ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
}
DrawBatch2::~DrawBatch2() {
    glBindVertexArray(0);

    if (m_vbo != 0) glDeleteBuffers(1, &m_vbo);
    if (m_vao != 0) glDeleteVertexArrays(1, &m_vao);
}

void DrawBatch2::begin() { clear(); }

void DrawBatch2::end(bool staticdraw) { glBindVertexArray(m_vao); create_batch(staticdraw); }

void DrawBatch2::clear() {
    m_vertices.clear();
    m_indices.clear();
}

bool DrawBatch2::is_empty() {
    return m_indices.size() == 0;
}

u32 DrawBatch2::add_vertex(Vertex v) {
    u32 idx = m_vertices.size();
    m_vertices.push_back(v);
    return idx;
}

u32 DrawBatch2::draw_tri(Vertex v1, Vertex v2, Vertex v3) {
    u32 fst_idx = add_vertex(v1);
    add_vertex(v2);
    add_vertex(v3);
    draw_tri(fst_idx, fst_idx + 1, fst_idx + 2);
    return fst_idx;
}

u32 DrawBatch2::draw_quad(Vertex tl, Vertex tr, Vertex br, Vertex bl) {
    u32 fst_idx = add_vertex(tl);
    add_vertex(tr);
    add_vertex(br);
    add_vertex(bl);
    draw_quad(fst_idx, fst_idx + 1, fst_idx + 2, fst_idx + 3);
    return fst_idx;
}

u32 DrawBatch2::draw_tri_strip(Vertex v) {
    u32 idx = add_vertex(v);
    draw_tri_strip(idx);
    return idx;
}

u32 DrawBatch2::draw_quad_strip(Vertex v1, Vertex v2) {
    u32 fst_idx = add_vertex(v1);
    add_vertex(v2);
    draw_quad_strip(fst_idx, fst_idx + 1);
    return fst_idx;
}

void DrawBatch2::draw_tri(u32 v1, u32 v2, u32 v3) {
    m_indices.push_back(v1);
    m_indices.push_back(v2);
    m_indices.push_back(v3);
}

void DrawBatch2::draw_quad(u32 tl, u32 tr, u32 br, u32 bl) {
    draw_tri(tl, tr, br);
    draw_tri(tl, br, bl);
}

void DrawBatch2::draw_tri_strip(u32 v) {
    draw_tri(v - 2, v - 1, v);
}

void DrawBatch2::draw_quad_strip(u32 v1, u32 v2) {
    draw_quad(v1 - 2, v2 - 2, v1, v2);
}

void DrawBatch2::render_batch() {
    glBindVertexArray(m_vao);

    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
}

void DrawBatch2::submit(bool clear_glyphs) {
    glBindVertexArray(m_vao);

    create_batch();

    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);

    if (clear_glyphs) clear();
}

void DrawBatch2::create_batch(bool staticdraw)
{
    if (m_vertices.empty() || m_indices.empty()) return;

    auto dt = staticdraw ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW;

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), m_vertices.data(), dt);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(Index), m_indices.data(), dt);
}

}
