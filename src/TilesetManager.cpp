#include "TilesetManager.h"
#include "Tileset.h"

namespace NSEngine {

    std::vector<Tileset*> TilesetManager::tilesets;

    TilesetManager::TilesetManager() {}
    TilesetManager::~TilesetManager() {}

    int TilesetManager::RegisterTileset(const char* name,int texID, int size)
    {
        tilesets.push_back((new Tileset)->Load(texID,size));
        return tilesets.size() -1;
    }

    Tileset* TilesetManager::getTileset(int i)
    {
        return tilesets[i];
    }

}
