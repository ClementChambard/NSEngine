#include "SpriteBatch.h"
#include "TextureManager.h"

#include <GL/glew.h>

namespace ns {

void set_blendmode(i32 i);

SpriteBatch::SpriteBatch() {
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
SpriteBatch::~SpriteBatch() {
    glBindVertexArray(0);

    if (m_vbo != 0) glDeleteBuffers(1, &m_vbo);
    if (m_vao != 0) glDeleteVertexArrays(1, &m_vao);
}

SpriteBatch* SpriteBatch::init() { return this; }

void SpriteBatch::begin() { clear(); }

void SpriteBatch::end(bool staticdraw) { glBindVertexArray(m_vao); create_render_batches(staticdraw); }

void SpriteBatch::clear() {
    m_renderBatches.clear();
    m_glyphs.clear();
}

bool SpriteBatch::is_empty() {
    return m_glyphs.size() == 0;
}

void SpriteBatch::draw(Texture* texture, Vertex tl, Vertex tr, Vertex br, Vertex bl, i32 blendmode) 
{
    m_glyphs.emplace_back(texture, tl, tr, br, bl, blendmode);
}

void SpriteBatch::draw(u32 texture, Vertex tl, Vertex tr, Vertex br, Vertex bl, i32 blendmode) 
{
    m_glyphs.emplace_back(TextureManager::get_texture(texture), tl, tr, br, bl, blendmode);
}

void SpriteBatch::render_batch() {

    glBindVertexArray(m_vao);

    for (usize i = 0; i < m_renderBatches.size(); i++)
    {
        if (i != 0)
        {
            if (m_renderBatches[i-1].texture != m_renderBatches[i].texture)
                m_renderBatches[i].texture->use();
            if (m_renderBatches[i-1].blendmode != m_renderBatches[i].blendmode)
                set_blendmode(m_renderBatches[i].blendmode);
        }
        else 
        {
            m_renderBatches[i].texture->use();
            set_blendmode(m_renderBatches[i].blendmode);
        }
        glDrawArrays(GL_TRIANGLES, m_renderBatches[i].offset, m_renderBatches[i].num_vertices);
    }

}

void SpriteBatch::submit(bool clear_glyphs) {
    glBindVertexArray(m_vao);

    create_render_batches(false);

    for (usize i = 0; i < m_renderBatches.size(); i++)
    {
        if (i != 0)
        {
            if (m_renderBatches[i-1].texture != m_renderBatches[i].texture)
                m_renderBatches[i].texture->use();
            if (m_renderBatches[i-1].blendmode != m_renderBatches[i].blendmode)
                set_blendmode(m_renderBatches[i].blendmode);
        }
        else 
        {
            m_renderBatches[i].texture->use();
            set_blendmode(m_renderBatches[i].blendmode);
        }
        glDrawArrays(GL_TRIANGLES, m_renderBatches[i].offset, m_renderBatches[i].num_vertices);
    }

    if (clear_glyphs) clear();
}

void SpriteBatch::bind_buffer()
{

}

void SpriteBatch::create_render_batches(bool)
{
    if (m_glyphs.empty()) return;

    usize n_vtx = m_glyphs.size() * 6;
    Vertex* vertices = new Vertex[n_vtx];

    i32 cv = 0; //current vertex
    m_renderBatches.emplace_back(0, 6, m_glyphs[0].texture, m_glyphs[0].blendmode);
    vertices[cv++] = m_glyphs[0].top_left;
    vertices[cv++] = m_glyphs[0].top_right;
    vertices[cv++] = m_glyphs[0].bottom_left;
    vertices[cv++] = m_glyphs[0].bottom_left;
    vertices[cv++] = m_glyphs[0].top_right;
    vertices[cv++] = m_glyphs[0].bottom_right;

    for (usize cg = 1; cg < m_glyphs.size(); cg++)
    {
        if (m_glyphs[cg].texture != m_glyphs[cg-1].texture || 
            m_glyphs[cg].blendmode != m_glyphs[cg-1].blendmode) {
            m_renderBatches.emplace_back(cv, 6, m_glyphs[cg].texture, m_glyphs[cg].blendmode);
        } else {
            m_renderBatches.back().num_vertices += 6;
        }
        vertices[cv++] = m_glyphs[cg].top_left;
        vertices[cv++] = m_glyphs[cg].top_right;
        vertices[cv++] = m_glyphs[cg].bottom_left;
        vertices[cv++] = m_glyphs[cg].bottom_left;
        vertices[cv++] = m_glyphs[cg].top_right;
        vertices[cv++] = m_glyphs[cg].bottom_right;
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, n_vtx * sizeof(Vertex), vertices, GL_DYNAMIC_DRAW);

    delete[] vertices;
}

void set_blendmode(i32 blendmode) {
    switch (blendmode) {
        case 0:
            glBlendEquation(GL_FUNC_ADD);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            break;
        case 1:
            glBlendEquation(GL_FUNC_ADD);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            break;
        case 2:
            glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            break;
        case 3:
            glBlendEquation(GL_FUNC_ADD);
            glBlendFunc(GL_ONE, GL_ZERO);
            break;
        case 4:
            glBlendEquation(GL_FUNC_ADD);
            glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE_MINUS_SRC_COLOR);
            break;
        case 5:
            glBlendEquation(GL_FUNC_ADD);
            glBlendFunc(GL_DST_COLOR, GL_ZERO);
            break;
        case 6:
            glBlendEquation(GL_FUNC_ADD);
            glBlendFunc(GL_ONE_MINUS_SRC_COLOR, GL_ONE_MINUS_SRC_ALPHA);
            break;
        case 7:
            glBlendEquation(GL_FUNC_ADD);
            glBlendFunc(GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA);
            break;
        case 8:
            glBlendEquation(GL_MIN);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            break;
        case 9:
            glBlendEquation(GL_MAX);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            break;
        case 10:
            glBlendEquation(GL_FUNC_SUBTRACT);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            break;
        default:
            glBlendEquation(GL_FUNC_ADD);
            glBlendFunc(GL_BLEND_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
}

}
