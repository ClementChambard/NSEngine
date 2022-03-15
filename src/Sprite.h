#ifndef SPRITE
#define SPRITE

#include <GL/glew.h>
#include "TextureManager.h"
#include "vertex.h"
#include <vector>
#include <glm/glm.hpp>

namespace NSEngine {

    struct Frame {
        Vertex tl, tr, br, bl;
    };

    class Sprite
    {

    public:
        Sprite();
        ~Sprite();

        Sprite* LoadSprite(const char* name);
        void DrawFrame(int frame, int x, int y, float xs, float ys, float rot, Color tl, Color tr, Color br, Color bl);

        int getNumOfFrames() { return numberOfFrames; }
        int getImageIndex(float frame);

        static int blendmode;

    private:
        int texId;
        std::vector<Frame> frames;
        std::vector<int> frameDuration;
        int numberOfFrames;

        void ConstructFrame(int width, int height, int offsetx, int offsety, UV uv1, UV uv2);
    };

}

#endif
