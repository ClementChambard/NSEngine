#include "Assets.h"
#include "NSEngine.h"

int defaultTexID;
int defaultTexture() { return defaultTexID; }
int defaultFontID;
int defaultFontTexture;
int defaultFont() { return defaultFontID; }

void gameassets::Init()
{
    defaultTexID = NSEngine::TextureManager::RegisterTexture("assets/engine/textures/defaultTexture.png");
    defaultFontTexture = NSEngine::TextureManager::RegisterTexture("assets/engine/textures/defaultFont_atlas.png");
    defaultFontID = NSEngine::SpriteManager::RegisterFont(defaultFontTexture, 64, 64, " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~ ", 95, 0, 0, -16);
}
