#include "PostProcessing.h"
#include "PPEffect.h"
#include <GL/glew.h>

namespace ns { 

const f32 PostProcessing::POSITIONS[] = {
    -1, 1, -1, -1, 1, 1, 1, -1
};
u32 PostProcessing::vbo_id = 0;
u32 PostProcessing::vao_id = 0;
std::vector<PPEffect*> PostProcessing::effects_queue;

void PostProcessing::init() {
    glCreateVertexArrays(1, &vao_id);
    glCreateBuffers(1, &vbo_id);
    glNamedBufferData(vbo_id, 8 * sizeof(f32), POSITIONS, GL_STATIC_DRAW);
    glEnableVertexArrayAttrib(vao_id, 0);
    glVertexArrayAttribBinding(vao_id, 0, 0);
    glVertexArrayAttribFormat(vao_id, 0, 2, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayVertexBuffer(vao_id, 0, vbo_id, 0, 2 * sizeof(f32));
}

void PostProcessing::do_post_processing(u32 color_texture) {
    start();
    for (auto e : effects_queue)
    {
        e->render(color_texture);
        color_texture = e->get_output_texture();
    }
    end();
}

void PostProcessing::cleanup() {
    for (auto e : effects_queue) { e->cleanup(); delete e; }
    glDeleteBuffers(1, &vbo_id);
    glDeleteVertexArrays(1, &vao_id);
}

void PostProcessing::add_step(PPEffect* eff) {
    effects_queue.push_back(eff);
}

void PostProcessing::start() {
    glBindVertexArray(vao_id);
    glDisable(GL_DEPTH_TEST);
}

void PostProcessing::end() {
    glEnable(GL_DEPTH_TEST);
    glBindVertexArray(0);
}

PPEffect::ImageRenderer::ImageRenderer(u32 w, u32 h) : fbo(new FrameBuffer(w, h)) {}
PPEffect::ImageRenderer::ImageRenderer() {}
PPEffect::ImageRenderer::~ImageRenderer() { if (fbo != nullptr) delete fbo; }

void PPEffect::ImageRenderer::render_quad()
{
    if (fbo != nullptr) fbo->bind();
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    if (fbo != nullptr) fbo->unbind();
}

u32 PPEffect::ImageRenderer::get_output_texture() const { if (fbo != nullptr) return fbo->get_color_texture(); return 0; }

FrameBuffer* PPEffect::ImageRenderer::get_fbo() const { return fbo; }

}
