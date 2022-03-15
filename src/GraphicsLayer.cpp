#include "GraphicsLayer.h"
#include "SpriteBatch.h"

namespace NSEngine {

    GraphicsLayer::~GraphicsLayer() { layerBatch->Dispose(); delete layerBatch; }

    void GraphicsLayer::render() { 
        layerBatch->renderBatch(depthTest); 
    }

}
