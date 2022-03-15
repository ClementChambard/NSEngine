#ifndef WATERRENDERER_INCLUDED_H
#define WATERRENDERER_INCLUDED_H

#include "RawModel.h"
#include "WaterTile.h"
#include "WaterShader.h"
#include <vector>

namespace NS3D {

    class WaterRenderer {
        public:
            WaterRenderer(WaterShader* shad);
            ~WaterRenderer();

            void render(const std::vector<const WaterTile*>& water, const Light* sun);

        private:
            WaterShader* shader;
            RawModel* quad;
            GLuint dudvMapTexID;
            GLuint normalMapTexID;

            void prepareRender(const Light* sun);
            void unbind();
            void setupVAO();
    };

}

#endif
