#ifndef GRAPHICSLAYER
#define GRAPHICSLAYER

#include "SpriteBatch.h"
#include "Tilemap.h"
#include "Error.h"
#include <string>
#include <vector>

namespace NSEngine {

    enum graphicsLayerType {GLT_TILES = 0, GLT_SPRITES, GLT_GUI};

    class GraphicsLayer {
        public:
            GraphicsLayer(graphicsLayerType t, int id) : type(t), id(id) { layerBatch = new SpriteBatch(); layerBatch->Init(); }
            ~GraphicsLayer();

            void LoadTileMap(int tilesetID, int width, std::vector<int> data) {
                if (type != GLT_TILES) { warning("trying to add tilemap to a non-tile graphics layer"); return; }
                tmap = new Tilemap(id);
                tmap->Load(tilesetID, width, data);
                info("tilemap added to graphics layer", id);
            }

            void EditTileMap(int x, int y, int tile) { tmap->ChangeTile(x, y, tile); }

            void render(); //options

            SpriteBatch* getBatch() { return layerBatch; }

            graphicsLayerType type;

            bool depthTest = false;
            bool is_static = false;
            int blendmode = 0;

        private:
            int id;
            SpriteBatch* layerBatch;
            Tilemap* tmap = nullptr;
    };

}

#endif
