#ifndef LAYERRENDERER_INCLUDED_H
#define LAYERRENDERER_INCLUDED_H

#include "AnimShader.h"
#include "GraphicsLayer.h"
#include <vector>

namespace NSEngine {

    class LayerRenderer {
        public:
            LayerRenderer(AnimShader* shad) : shader(shad) {}
            ~LayerRenderer() { delete shader; }

            void addLayer(GraphicsLayer* l) { layers.push_back(l); }
            void setRenderFunction(void (*func)(LayerRenderer*)) { renderFunction = func; }

            void initFrame();
            void renderLayers();

            AnimShader* getShader() { return shader; }

        private:
            AnimShader* shader = nullptr;

            void (*renderFunction)(LayerRenderer*) = nullptr;

            std::vector<GraphicsLayer*> layers;
            
    };

}

#endif
