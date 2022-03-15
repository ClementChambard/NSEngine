#ifndef TILEMAP
#define TILEMAP

#include "SpriteBatch.h"
#include "TilesetManager.h"

namespace NSEngine {

    class Tilemap {
        public:
            Tilemap(int layer);
            ~Tilemap();

            Tilemap* Load(int tileset, int width, std::vector<int> data);
            void ChangeTile(int x, int y, int tile);
            void Draw();

        private:
            SpriteBatch* batch;
            Tileset* ts;
            int width;
            std::vector<int> data;
    };

}

#endif
