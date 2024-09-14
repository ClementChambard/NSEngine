#include "TerrainRenderer.h"
#include <GL/glew.h>

namespace ns::n3d {

TerrainRenderer::TerrainRenderer(TerrainShader* shad) {
    shader = shad;
    shader->use();
    shader->connect_texture_units();
}

void TerrainRenderer::render(const std::vector<const Terrain*>& terrains, const std::vector<const Light*>& lights) {
    shader->use();
    // shader->set_projection_matrix(ns::activeCamera3D()->getProjection());
    // shader->set_view_matrix(ns::activeCamera3D()->getView());
    shader->set_lights(lights);

    for (auto t : terrains) {
        prepare_terrain(t);
        load_model_matrix(t);
        glDrawElements(GL_TRIANGLES, t->get_model()->get_vertex_count(), GL_UNSIGNED_INT, 0);
        unbind_terrain();
    }
}


void TerrainRenderer::prepare_terrain(const Terrain* terrain)
{
    RawModel* model = terrain->get_model();
    glBindVertexArray(model->get_vao_id());
    shader->set_shine_vars(1, 0);
    bind_textures(terrain);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
}

void TerrainRenderer::bind_textures(const Terrain* t)
{
    TerrainTexture* ttex = t->get_texture();
    glActiveTexture(GL_TEXTURE0);
    ttex->get_bg_tex()->use();
    glActiveTexture(GL_TEXTURE1);
    ttex->get_r_tex()->use();
    glActiveTexture(GL_TEXTURE2);
    ttex->get_g_tex()->use();
    glActiveTexture(GL_TEXTURE3);
    ttex->get_b_tex()->use();
    glActiveTexture(GL_TEXTURE4);
    t->get_blendmap()->use();
}

void TerrainRenderer::unbind_terrain()
{
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glBindVertexArray(0);
}

void TerrainRenderer::load_model_matrix(const Terrain* terrain)
{
    mat4 model_matrix = mat4::mk_translate({terrain->get_x(), 0, terrain->get_z()});
    shader->set_model_matrix(model_matrix);
}

}
