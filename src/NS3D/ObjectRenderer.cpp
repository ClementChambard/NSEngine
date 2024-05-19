#include "ObjectRenderer.h"
#include "../NSEngine.hpp"

namespace NS3D {

void ObjectRenderer::render(const std::map<const TexturedModel *, std::vector<const Object3D *> >& objects, const std::vector<const Light*>& lights)
{
    shader->start();
    shader->SetLights(lights);
    shader->SetProjectionMatrix(ns::activeCamera3D()->getProjection());
    shader->SetViewMatrix(ns::activeCamera3D()->getView());

    for (auto model : objects)
    {
        prepareTexturedModel(model.first);
        for (auto object : model.second)
        {
            prepareInstance(object);

            glDrawElements(GL_TRIANGLES, model.first->getRawModel()->getVertexCount(),
                    GL_UNSIGNED_INT, 0);

        }
        unbindTexturedModel();
    }

    shader->stop();

}

void ObjectRenderer::renderOutlined(const Object3D *obj)
{
    oshader->start();
    oshader->SetProjectionMatrix(ns::activeCamera3D()->getProjection());
    oshader->SetViewMatrix(ns::activeCamera3D()->getView());
    oshader->SetColor({1,0.6,0.1});
    glLineWidth(3);
    glCullFace(GL_FRONT);
    glPolygonMode(GL_BACK, GL_LINE);

    glBindVertexArray(obj->getModel()->getRawModel()->getVaoID());
    glEnableVertexAttribArray(0);
    oshader->SetModelMatrix(obj->getMatrix());
    glDrawElements(GL_TRIANGLES, obj->getModel()->getRawModel()->getVertexCount(),
            GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(0);
    glBindVertexArray(0);

    glLineWidth(1);
    glCullFace(GL_BACK);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    oshader->stop();
}

void ObjectRenderer::prepareTexturedModel(const TexturedModel* model)
{
    glBindVertexArray(model->getRawModel()->getVaoID());
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    if (model->getModelTexture()->getHasTransparency()) glDisable(GL_CULL_FACE);
    shader->SetNumberOfRows(model->getModelTexture()->getNumberOfRows());
    shader->SetTextureVars(model->getModelTexture()->getShineDamper(),model->getModelTexture()->getReflectivity(),
            model->getModelTexture()->getUseFakeNormals());
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, model->getModelTexture()->getID());
}

void ObjectRenderer::prepareInstance(const Object3D* object)
{
    shader->SetModelMatrix(object->getMatrix());
    shader->SetOffset(glm::vec2(object->getTextureXOffset(),object->getTextureYOffset()));
}

void ObjectRenderer::unbindTexturedModel()
{
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glBindVertexArray(0);
}

}
