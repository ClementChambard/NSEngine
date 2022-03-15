#ifndef TILESET
#define TILESET

#include "SpriteBatch.h"
namespace NSEngine {

    class Tileset {
        public:
            Tileset();
            ~Tileset();

            Tileset* Load(int texID, int tileSize);
            void DrawTileToBatch(int tile, float x, float y, SpriteBatch* batch, float depth = 0.f);
        private:
            int texID = -1;
            int texXsize = 0, texYsize = 0;
            int tileSize = 0;
            int xtiles = 0, ytiles = 0;
            float uvWidth = 1;
            float uvHeight = 1;
            UV getUV(int tile, int xx, int yy);
    };

}
#endif
