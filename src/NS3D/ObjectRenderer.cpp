#include "ObjectRenderer.h"
#include "../NSEngine.h"

namespace NS3D {

    void ObjectRenderer::render(const std::map<const TexturedModel *, std::vector<const Object3D *> >& objects, const std::vector<const Light*>& lights)
    {
        shader->start();
        shader->SetLights(lights);
        shader->SetProjectionMatrix(NSEngine::activeCamera3D()->getProjection());
        shader->SetViewMatrix(NSEngine::activeCamera3D()->getView());

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

    void ObjectRenderer::prepareTexturedModel(const TexturedModel* model)
    {
        glBindVertexArray(model->getRawModel()->getVaoID());
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        if (model->getModelTexture()->getHasTransparency()) NSEngine::toggleCulling(false);
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
        NSEngine::toggleCulling(true);
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
        glBindVertexArray(0);
    }

}
