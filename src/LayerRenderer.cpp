#include "LayerRenderer.h"
#include "NSEngine.h"

namespace NSEngine {

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
        toggleCulling(false);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        for (auto l : layers)
        {
            float mi=1000000.f, ma=1000000.f;
            glm::vec4 col = {0,0,0,0};
            l->end();
            shader->SetProjectionMatrix(cam3dexists?activeCamera3D()->getProjection(l->is_static):glm::mat4(1.f));
            shader->SetViewMatrix(cam3dexists?activeCamera3D()->getView(l->is_static):glm::mat4(1.f));
            if (activeCamera3D() != nullptr && !l->is_static) col = engineData::cam3d->getFog(mi,ma);
            shader->SetFog(mi, ma, col);
            l->renderBatch();
        }
        toggleCulling(true);
        
        shader->stop();
        TextureManager::ResetTexture();
        glDisable(GL_BLEND);
    }

}
