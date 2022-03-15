#include "GameSprites.h"

sprite_data::sprite_data(int t, int w, int h, float u1, float v1, float u2, float v2)
{
    this->tex_id = t;
    this->w = w;
    this->h = h;
    this->u1 = u1;
    this->v1 = v1;
    this->u2 = u2;
    this->v2 = v2;
}

std::vector<sprite_data> GS_ sprs;

int GS_ AddSprite(int tex_id, int x, int y, int w, int h)
{
    int id = sprs.size();
    NSEngine::UV uv1 = NSEngine::TextureManager::GetUVAt(tex_id, x, y);
    NSEngine::UV uv2 = NSEngine::TextureManager::GetUVAt(tex_id, x+w, y+h);
    sprs.push_back(sprite_data(tex_id,w,h,uv1.u,uv1.v,uv2.u,uv2.v));
    return id;
}

int GS_ AddSprites(int tex_id, int xstart, int ystart, int nx, int ny, int w, int h)
{
    int ret;
    for (int i = 0; i < ny; i++)
        for (int j = 0; j < nx; j++)
            if (i == 0 && j == 0) ret = AddSprite(tex_id, xstart, ystart, w, h);
            else AddSprite(tex_id, xstart + j * w, ystart + i * h, w, h);
    return ret;
}

sprite_data* sprite(unsigned int i)
{
    if (i >= GS_ sprs.size()) return nullptr;
    return &(GS_ sprs[i]);
}
