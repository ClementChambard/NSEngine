#include "Assets.h"
#include "NSEngine.h"


int defaultTexID;
int defaultTexture() { return defaultTexID; }
int defaultFontID;
int defaultFont() { return defaultFontID; }

void gameassets::Init()
{
    defaultTexID = NSEngine::TextureManager::RegisterTexture(        "assets/engine/textures/defaultTexture.png");
    defaultFontID = NSEngine::SpriteManager::RegisterFont(        "assets/engine/fonts/Lato-Regular.ttf, 10");
}