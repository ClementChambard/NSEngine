#ifndef SPRITEMANAGER
#define SPRITEMANAGER

#include "Sprite.h"
#include "SpriteFont.h"
#include <vector>

namespace NSEngine {

    class SpriteManager
    {

        public:
            static int RegisterSprite(const char* name);
            static int RegisterFont(const char* name, int size);
            static int RegisterFont(const char* name);
            static int RegisterFont(int texID, int width, int height, const char* chars, int numberofchars, int offsetX, int offsetY, int writepadding);

            static void draw_sprite(int sprite_index, int frame, int x, int y, float xs, float ys, float rot, Color ctl, Color ctr, Color cbr, Color cbl);
            static void draw_text(int font_id, const char* text, glm::vec2 pos, glm::vec2 scale, Color col);
            static int getImageIndexFromFrame(int sprite, float& frame);
 
            static void cleanUp();

        private:
            static std::vector<Sprite*> sprites;
            static std::vector<SpriteFont*> spritefonts;
    };

}
#endif

