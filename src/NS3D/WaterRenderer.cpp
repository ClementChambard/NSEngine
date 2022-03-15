#include "WaterRenderer.h"
#include "WaterFrameBuffers.h"
#include "../math/math.h"
#include "../NSEngine.h"
#include "Loader3D.h"

namespace NS3D {

    WaterRenderer::WaterRenderer(WaterShader* shad) : shader(shad)
    {
        setupVAO();
        shader->start();
        shader->connectTextures();
        shader->stop();
        dudvMapTexID = NSEngine::ImageLoader::loadTexture("assets/engine/textures/waterDudvMap.png").id;
        normalMapTexID = NSEngine::ImageLoader::loadTexture("assets/engine/textures/waterNormalMap.png").id;
    }

    WaterRenderer::~WaterRenderer()
    {
        glDeleteTextures(1, &dudvMapTexID);
        glDeleteTextures(1, &normalMapTexID);
        delete shader;
    }

    void WaterRenderer::render(const std::vector<const WaterTile*>& water, const Light* sun)
    {
        prepareRender(sun);
        for (auto w : water)
        {
            glm::mat4 modelMat = math::makeTransformationMatrix({w->getX(), w->getHeight(), w->getZ()},{0,0,0},{w->getW(),1,w->getH()});
            shader->SetModelMatrix(modelMat);
            glDrawArrays(GL_TRIANGLES, 0, quad->getVertexCount());
        }
        unbind();
    }

    void WaterRenderer::prepareRender(const Light* sun)
    {
        static float mvFact1 = 0.f, mvFact2 = 0.f;
        mvFact1 += 0.003f; mvFact1 = math::mod(mvFact1,1.0f);
        mvFact2 += 0.001f; mvFact2 = math::mod(mvFact2,1.0f);
        shader->start();
        shader->SetProjectionMatrix(NSEngine::activeCamera3D()->getProjection());
        shader->SetViewMatrix(NSEngine::activeCamera3D()->getView());
        shader->SetCameraPosition(NSEngine::activeCamera3D()->getPosition());
        shader->SetMoveFactor({mvFact1,mvFact2});
        if (sun != nullptr)
            shader->SetSun(sun);
        else
        {
            sun = new Light({0,0,0},{0,0,0});
            shader->SetSun(sun);
            delete sun; sun = nullptr;
        }
        glBindVertexArray(quad->getVaoID());
        glEnableVertexAttribArray(0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, WaterFrameBuffers::getReflectionTexture());
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, WaterFrameBuffers::getRefractionTexture());
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, dudvMapTexID);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, normalMapTexID);
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, WaterFrameBuffers::getRefractionDepthTexture());

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    void WaterRenderer::unbind()
    {
        glDisable(GL_BLEND);
        glDisableVertexAttribArray(0);
        glBindVertexArray(0);
        shader->stop();
    }

    //TODO Delete vao plz
    void WaterRenderer::setupVAO()
    {
        float vertices[] = { -1, -1, -1, 1, 1, -1, 1, -1, -1, 1, 1, 1 };
        quad = Loader3D::loadToVAO(vertices, 12, 2);
    }
}
