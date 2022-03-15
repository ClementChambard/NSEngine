#include "Tilemap.h"
#include "NSEngine.h"
#include "SpriteBatch.h"
#include "TilesetManager.h"

#include <iostream>

namespace NSEngine {

    Tilemap::Tilemap(int layer) { batch = engineData::layers[layer]->getBatch(); }
    Tilemap::~Tilemap() {}

    Tilemap* Tilemap::Load(int tileset, int width, std::vector<int> data) {

        this->ts = TilesetManager::getTileset(tileset);
        this->data = data;
        for (int i = 0; i < data.size(); i++) this->data[i] = data[i];
        this->width = width;

        Draw();

        return this;
    }

    void Tilemap::ChangeTile(int x, int y, int tile) {

        data[y*width+x] = tile;

        Draw();
    }

    void Tilemap::Draw() {

        batch->begin();

        int x = 0, y = 0;
        for (int tile : data)
        {
            ts->DrawTileToBatch(tile, x, y, batch, 0);
            x++;
            if (x == width)
            {
                x = 0;
                y++;
            }
        }

        batch->end(true);
    }

}
