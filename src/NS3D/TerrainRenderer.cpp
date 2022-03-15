#include "TerrainRenderer.h"
#include "../math/math.h"
#include "../NSEngine.h"

namespace NS3D {

    TerrainRenderer::TerrainRenderer(TerrainShader* shad)
    {
        shader = shad;
        shader->start();
        shader->connectTextureUnits();
        shader->stop();
    }

    void TerrainRenderer::render(const std::vector<const Terrain*>& terrains, const std::vector<const Light*>& lights)
    {
        shader->start();
        shader->SetProjectionMatrix(NSEngine::activeCamera3D()->getProjection());
        shader->SetViewMatrix(NSEngine::activeCamera3D()->getView());
        shader->SetLights(lights);

        for (auto t : terrains)
        {
            prepareTerrain(t);
            loadModelMatrix(t);
            glDrawElements(GL_TRIANGLES, t->getModel()->getVertexCount(), GL_UNSIGNED_INT, 0);
            unbindTerrain();
        }

        shader->stop();
    }


    void TerrainRenderer::prepareTerrain(const Terrain* terrain)
    {
        RawModel* model = terrain->getModel();
        glBindVertexArray(model->getVaoID());
        shader->SetShineVars(1, 0);
        bindTextures(terrain);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
    }

    void TerrainRenderer::bindTextures(const Terrain* t)
    {
        TerrainTexture* ttex = t->getTexture();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ttex->getBgTexID());
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, ttex->getRTexID());
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, ttex->getGTexID());
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, ttex->getBTexID());
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, t->getBlendMap());
    }

    void TerrainRenderer::unbindTerrain()
    {
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
        glBindVertexArray(0);
    }

    void TerrainRenderer::loadModelMatrix(const Terrain* terrain)
    {
        glm::mat4 modelMatrix = math::makeTransformationMatrix({terrain->getX(), 0, terrain->getZ()},
                {0, 0, 0}, {1, 1, 1});
        shader->SetModelMatrix(modelMatrix);
    }

}
