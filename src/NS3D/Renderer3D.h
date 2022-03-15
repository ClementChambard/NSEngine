#ifndef RENDERER3D_INCLUDED_H
#define RENDERER3D_INCLUDED_H

#include <GL/glew.h>
#include "ObjectRenderer.h"
#include "TerrainRenderer.h"
#include "SkyboxRenderer.h"
#include "WaterRenderer.h"
#include "../FrameBuffer.h"

namespace NS3D {

    class Renderer3D {
        public:
            Renderer3D();
            ~Renderer3D();

            void prepare();
            void render(NSEngine::FrameBuffer* fbo = nullptr);

            void processTerrain(const Terrain* terrain);
            void processObject(const Object3D* object);
            void processWaterTile(const WaterTile* water);
            void processLight(const Light* l);

            void setSkybox(GLuint texID) { skyRenderer->setTexture(texID); }
            void setSkyColor(glm::vec3 color);

        private:
            ObjectRenderer* objRenderer;
            TerrainRenderer* terRenderer;
            SkyboxRenderer* skyRenderer;
            WaterRenderer* watRenderer;

            glm::vec3 skyCol = glm::vec3(0,0,0);

            std::map<const TexturedModel*, std::vector<const Object3D*>> objectsToRender;
            std::vector<const Terrain*> terrainsToRender;
            std::vector<const WaterTile*> waterToRender;
            std::vector<const Light*> lights;

    };

}

#endif
