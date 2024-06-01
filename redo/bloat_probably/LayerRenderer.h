#ifndef LAYERRENDERER_INCLUDED_H
#define LAYERRENDERER_INCLUDED_H

#include "AnimShader.h"
#include "SpriteBatch.h"
#include <vector>

namespace ns {

class LayerRenderer {
    public:
        LayerRenderer(AnimShader* shad) : shader(shad) {}
        ~LayerRenderer() { delete shader; }

        void addLayer(SpriteBatch* l) { layers.push_back(l); }
        void setRenderFunction(void (*func)(LayerRenderer*)) { renderFunction = func; }

        void initFrame();
        void renderLayers();

        AnimShader* getShader() { return shader; }

    private:
        AnimShader* shader = nullptr;

        void (*renderFunction)(LayerRenderer*) = nullptr;

        std::vector<SpriteBatch*> layers;
        
};

}

#endif
