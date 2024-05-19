#include "LayerRenderer.h"
#include "./Camera3D.h"
#include "./TextureManager.h"
#include "./defines.h"

namespace ns {

extern Camera3D* activeCamera3D();

void LayerRenderer::initFrame()
{
    for (auto l : layers) l->begin();
}

void LayerRenderer::renderLayers()
{
    if (renderFunction != nullptr)
    {
        renderFunction(this);
        return;
    }
    shader->start();
    bool cam3dexists = activeCamera3D()!=nullptr;
    shader->SetCameraPosition(cam3dexists?activeCamera3D()->getPosition():glm::vec3(0,0,0));
    glActiveTexture(GL_TEXTURE0);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    for (auto l : layers)
    {
        f32 mi=1000000.f, ma=1000000.f;
        glm::vec4 col = {0,0,0,0};
        l->end();
        if (l->is_static)
        {
            shader->SetProjectionMatrix(glm::mat4(1.f));
            shader->SetViewMatrix(cam3dexists?activeCamera3D()->getCamStatic():glm::mat4(1.f));
        }
        else
        {
            shader->SetProjectionMatrix(cam3dexists?activeCamera3D()->getProjection(l->is_static):glm::mat4(1.f));
            shader->SetViewMatrix(cam3dexists?activeCamera3D()->getView(l->is_static):glm::mat4(1.f));
        }
        if (activeCamera3D()!=nullptr && !l->is_static) col = activeCamera3D()->getFog(mi,ma);
        shader->SetFog(mi, ma, col);
        l->renderBatch();
    }
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    
    shader->stop();
    TextureManager::ResetTexture();
    glDisable(GL_BLEND);
}

}
