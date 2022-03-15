#ifndef SPRITEFONT
#define SPRITEFONT

#include "vertex.h"
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <vector>
#include "SpriteBatch.h"

namespace NSEngine {

    struct GLTexture;

    #define FIRST_PRINTABLE_CHAR ((char) 32)
    #define LAST_PRINTABLE_CHAR ((char) 126)

    enum font_align { fa_left, fa_center, fa_right, fa_top, fa_bottom };

    struct CharGlyph {
        char character;
        glm::vec4 uvRect;
        glm::vec2 size;
    };

    class SpriteFont {
        public:
            SpriteFont();
            SpriteFont(const char* font, int size);
            SpriteFont(const char* font);
            SpriteFont(int texID, int width, int height, const char* chars,int numberofchars, int offsetX = 0, int offsetY = 0, int padding = 0);
            ~SpriteFont();

            int getFontHeight() const { return fontHeight; }

            glm::vec2 measure(const char* s);

            void draw(const char* s, glm::vec2 position, glm::vec2 scaling, float depth, Color tint);

        private:
            
            bool fromString = false;
            uint8_t nbofglyphs = 0;
            static std::vector<int>* createRows(glm::ivec4* rects, int rectsLength, int r, int padding, int& w);

            int padding = 0;
            int fontHeight = 0;
            CharGlyph* glyphs;

            unsigned int texID;

    };

    extern void draw_set_halign(font_align fa);
    extern void draw_set_valign(font_align fa);

}

#endif
