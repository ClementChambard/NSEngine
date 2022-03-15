#ifndef GAMESPRITES_H
#define GAMESPRITES_H

#include <vector>
#include "TextureManager.h"

struct sprite_data {
    sprite_data(int t, int w, int h, float u1, float v1, float u2, float v2);
    int tex_id = 0;
    int w = 0;
    int h = 0;
    float u1 = 0.f;
    float v1 = 0.f;
    float u2 = 0.f;
    float v2 = 0.f;
};

#define GS_ GameSprites::

class GameSprites {
    public:
        static int AddSprite(int tex_id, int x, int y, int w, int h);
        static int AddSprites(int tex_id, int xstart, int ystart, int nx, int ny, int w, int h);
        static std::vector<sprite_data> sprs;
};


extern sprite_data* sprite(unsigned int i);

#endif
