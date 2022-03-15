#include "Tileset.h"
#include "Error.h"
#include "TextureManager.h"
#include "vertex.h"
#include <GL/glew.h>
#include <string>

namespace NSEngine {

    Tileset::Tileset() {}
    Tileset::~Tileset() {}

    Tileset* Tileset::Load(int texID, int tileSize) {

        this->texID = texID;
        this->tileSize = tileSize;
        TextureManager::GetTextureSize(texID, texXsize, texYsize);
        xtiles = texXsize / tileSize;
        ytiles = texYsize / tileSize;
        uvWidth = 1.f / (float)xtiles;
        uvHeight = 1.f / (float)ytiles;
        return this;
    }

    void Tileset::DrawTileToBatch(int tile, float x, float y, SpriteBatch* batch, float depth)
    {
        Vertex tl = { {x * tileSize,            y * tileSize,            depth}, c_white, getUV(tile,0,1) };
        Vertex tr = { {x * tileSize + tileSize, y * tileSize,            depth}, c_white, getUV(tile,1,1) };
        Vertex br = { {x * tileSize + tileSize, y * tileSize + tileSize, depth}, c_white, getUV(tile,1,0) };
        Vertex bl = { {x * tileSize,            y * tileSize + tileSize, depth}, c_white, getUV(tile,0,0) };

        batch->draw(texID, tl, tr, br, bl);
    }

    UV Tileset::getUV(int tile, int xx, int yy)
    {
        float u = (float)uvWidth * (float)(tile % xtiles);
        float v = (float)uvHeight * (float)(tile / xtiles);

        return {u + xx * uvWidth, v + yy * uvHeight};
    }

}
