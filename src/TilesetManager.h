#ifndef TILESETMANAGER
#define TILESETMANAGER

#include "Tileset.h"
#include <vector>

namespace NSEngine {

    class TilesetManager {
        public:
            TilesetManager();
            ~TilesetManager();

            static int RegisterTileset(const char* name, int texID, int size);
            static Tileset* getTileset(int i);
        private:
            static std::vector<Tileset*> tilesets;
    };

}

#endif
