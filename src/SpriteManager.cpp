#include "SpriteManager.h"
#include "Assets.h"
#include "NSEngine.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"
#include "vertex.h"
#include "math/math.h"
#include <glm/fwd.hpp>

namespace NSEngine {


    std::vector<Sprite*> SpriteManager::sprites;
    std::vector<SpriteFont*> SpriteManager::spritefonts;

    int SpriteManager::RegisterSprite(const char* name)
    {
        sprites.push_back((new Sprite())->LoadSprite(name));
        return sprites.size() -1;
    }
    int SpriteManager::RegisterFont(const char* name, int size)
    {
        spritefonts.push_back(new SpriteFont(name,size));
        return spritefonts.size() -1;
    }
    int SpriteManager::RegisterFont(const char* name)
    {
        spritefonts.push_back(new SpriteFont(name));
        return spritefonts.size() -1;
    }
    int SpriteManager::RegisterFont(int texID, int width, int height, const char* chars, int numberofchars, int offsetX, int offsetY, int writepadding)
    {
        spritefonts.push_back(new SpriteFont(texID, width, height, chars, numberofchars, offsetX, offsetY, writepadding));
        return spritefonts.size() -1;
    }

    void SpriteManager::draw_sprite(int sprite_index, int frame, int x, int y, float xs, float ys, float rot, Color ctl, Color ctr, Color cbr, Color cbl)
    {
        sprites[sprite_index]->DrawFrame(frame,x,y,xs,ys,rot,ctl,ctr,cbr,cbl);
    }

    void SpriteManager::draw_text(int font_id, const char* text, glm::vec2 pos, glm::vec2 scale, Color col)
    {
        spritefonts[font_id]->draw(text, pos, scale, 0, col);
    }


    int SpriteManager::getImageIndexFromFrame(int sprite, float& frame)
    {
        int nboffr = sprites[sprite]->getNumOfFrames();
        if (frame >= nboffr) frame -= nboffr;
        if (frame < 0) frame += nboffr;
        return sprites[sprite]->getImageIndex(frame);
    }

    void SpriteManager::cleanUp()
    {
        for (auto s : sprites) delete s;
        for (auto f : spritefonts) delete f;
    }

}
