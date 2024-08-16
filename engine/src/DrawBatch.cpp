#include "DrawBatch.h"

#include <GL/glew.h>

namespace ns {

DrawBatch::DrawBatch() {
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
}
DrawBatch::~DrawBatch() {
    glBindVertexArray(0);

    if (m_vbo != 0) glDeleteBuffers(1, &m_vbo);
    if (m_vao != 0) glDeleteVertexArrays(1, &m_vao);
}

void DrawBatch::begin() { clear(); }

void DrawBatch::end(bool staticdraw) { glBindVertexArray(m_vao); create_batch(staticdraw); }

void DrawBatch::clear() {
    m_quads.clear();
}

bool DrawBatch::is_empty() {
    return m_quads.size() == 0;
}

void DrawBatch::draw(Vertex tl, Vertex tr, Vertex br, Vertex bl) 
{
    m_quads.push_back({tl, tr, br, bl});
}

void DrawBatch::render_batch() {

    glBindVertexArray(m_vao);

    glDrawArrays(GL_TRIANGLES, 0, m_quads.size() * 6);
}

void DrawBatch::submit(bool clear_glyphs) {
    glBindVertexArray(m_vao);

    create_batch(false);

    glDrawArrays(GL_TRIANGLES, 0, m_quads.size() * 6);

    if (clear_glyphs) clear();
}

void DrawBatch::create_batch(bool)
{
    if (m_quads.empty()) return;

    usize n_vtx = m_quads.size() * 6;
    Vertex* vertices = new Vertex[n_vtx];

    i32 cv = 0; //current vertex
    for (auto& q : m_quads) {
        vertices[cv++] = q.data[0];
        vertices[cv++] = q.data[1];
        vertices[cv++] = q.data[3];
        vertices[cv++] = q.data[3];
        vertices[cv++] = q.data[1];
        vertices[cv++] = q.data[2];
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, n_vtx * sizeof(Vertex), vertices, GL_DYNAMIC_DRAW);

    delete[] vertices;
}

}
