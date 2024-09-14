#include "WaterRenderer.h"
#include "WaterFrameBuffers.h"
#include <math/math.hpp>
#include <GL/glew.h>
#include "Loader3D.h"

namespace ns::n3d {

WaterRenderer::WaterRenderer(WaterShader* shad) : shader(shad)
{
    setup_vao();
    shader->use();
    shader->connect_textures();
    dudv_map_tex = new Texture("assets/n3d/textures/waterDudvMap.png");
    normal_map_tex = new Texture("assets/n3d/textures/waterNormalMap.png");
}

WaterRenderer::~WaterRenderer()
{
    delete dudv_map_tex;
    delete normal_map_tex;
    delete shader;
    delete quad;
}

void WaterRenderer::render(const std::vector<const WaterTile*>& water, const Light* sun)
{
    prepare_render(sun);
    for (auto w : water)
    {
        mat4 model_mat = ns::mat4::mk_scale({w->get_w(), 1, w->get_h()}).translate({w->get_x(), w->get_height(), w->get_z()});
        shader->set_model_matrix(model_mat);
        glDrawArrays(GL_TRIANGLES, 0, quad->get_vertex_count());
    }
    unbind();
}

void WaterRenderer::prepare_render(const Light* sun)
{
    static f32 mv_fact1 = 0.f, mv_fact2 = 0.f;
    mv_fact2 += 0.003f; mv_fact2 = ns::mod(mv_fact1,1.0f);
    mv_fact2 += 0.001f; mv_fact2 = ns::mod(mv_fact2,1.0f);
    shader->use();
    // shader->set_projection_matrix(ns::activeCamera3D()->getProjection());
    // shader->set_view_matrix(ns::activeCamera3D()->getView());
    // shader->set_camera_position(ns::activeCamera3D()->getPosition());
    shader->set_move_factor({mv_fact2, mv_fact2});
    if (sun != nullptr)
        shader->set_sun(sun);
    else
    {
        sun = new Light({0,0,0},{0,0,0});
        shader->set_sun(sun);
        delete sun; sun = nullptr;
    }
    glBindVertexArray(quad->get_vao_id());
    glEnableVertexAttribArray(0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, WaterFrameBuffers::get_reflection_texture());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, WaterFrameBuffers::get_refraction_texture());
    glActiveTexture(GL_TEXTURE2);
    dudv_map_tex->use();
    glActiveTexture(GL_TEXTURE3);
    normal_map_tex->use();
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, WaterFrameBuffers::get_refraction_depth_texture());

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void WaterRenderer::unbind()
{
    glDisable(GL_BLEND);
    glDisableVertexAttribArray(0);
    glBindVertexArray(0);
}

//TODO Delete vao plz
void WaterRenderer::setup_vao()
{
    f32 vertices[] = { -1, -1, -1, 1, 1, -1, 1, -1, -1, 1, 1, 1 };
    quad = Loader3D::load_to_vao(vertices, 12, 2);
}
}
