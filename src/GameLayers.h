#ifndef GAMELAYERS
#define GAMELAYERS

#include "GraphicsLayer.h"
#include "Window.h"
#include "LayerRenderer.h"

#define GL_ GameLayers::

class GameLayers {
    public:
        static void Init(NSEngine::LayerRenderer* l);
        static int L1;
        static int L2;
        static int L3;
        static int L4;
        static int L5;
        static int L6;
        static int L7;
        static int L8;
        static int L9;
        static int L10;
        static int L11;
        static int L12;
        static int L13;
        static int L14;
        static int L15;
        static int L16;
        static int L17;
        static int L18;
        static int L19;
        static int L20;
        static int L21;
        static int L22;
        static int L23;
        static int L24;
        static int L25;
        static int L26;
        static int L27;
        static int L28;
        static int L29;
        static int L30;

        static void Render(NSEngine::LayerRenderer* w);
        static void Render(NSEngine::Window* w);
};

extern NSEngine::GraphicsLayer* layer(int i);

#endif
