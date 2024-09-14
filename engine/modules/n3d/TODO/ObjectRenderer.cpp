#include "ObjectRenderer.h"
#include <GL/glew.h>

namespace ns::n3d {

void ObjectRenderer::render(const std::map<const TexturedModel *, std::vector<const Object3D *> >& objects, const std::vector<const Light*>& lights)
{
    shader->use();
    shader->set_lights(lights);
    // shader->set_projection_matrix(ns::activeCamera3D()->getProjection());
    // shader->set_view_matrix(ns::activeCamera3D()->getView());

    for (auto model : objects)
    {
        prepare_textured_model(model.first);
        for (auto object : model.second)
        {
            prepare_instance(object);

            glDrawElements(GL_TRIANGLES, model.first->get_raw_model()->get_vertex_count(),
                    GL_UNSIGNED_INT, 0);

        }
        unbind_textured_model();
    }
}

void ObjectRenderer::render_outlined(const Object3D *obj)
{
    oshader->use();
    // oshader->set_projection_matrix(ns::activeCamera3D()->getProjection());
    // oshader->set_view_matrix(ns::activeCamera3D()->getView());
    oshader->set_color({1,0.6,0.1});
    glLineWidth(3);
    glCullFace(GL_FRONT);
    glPolygonMode(GL_BACK, GL_LINE);

    glBindVertexArray(obj->get_model()->get_raw_model()->get_vao_id());
    glEnableVertexAttribArray(0);
    oshader->set_model_matrix(obj->get_matrix());
    glDrawElements(GL_TRIANGLES, obj->get_model()->get_raw_model()->get_vertex_count(),
            GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(0);
    glBindVertexArray(0);

    glLineWidth(1);
    glCullFace(GL_BACK);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void ObjectRenderer::prepare_textured_model(const TexturedModel* model)
{
    glBindVertexArray(model->get_raw_model()->get_vao_id());
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    if (model->get_model_texture()->get_has_transparency()) glDisable(GL_CULL_FACE);
    shader->set_number_of_rows(model->get_model_texture()->get_number_of_rows());
    shader->set_texture_vars(model->get_model_texture()->get_shine_damper(),model->get_model_texture()->get_reflectivity(),
            model->get_model_texture()->get_use_fake_normals());
    glActiveTexture(GL_TEXTURE0);
    model->get_model_texture()->get_texture()->use();
}

void ObjectRenderer::prepare_instance(const Object3D* object)
{
    shader->set_model_matrix(object->get_matrix());
    shader->set_offset(vec2(object->get_texture_xoffset(), object->get_texture_yoffset()));
}

void ObjectRenderer::unbind_textured_model()
{
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glBindVertexArray(0);
}

}
