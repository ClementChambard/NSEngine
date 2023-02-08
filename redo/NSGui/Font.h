#ifndef FONT_H_
#define FONT_H_

#include <GL/glew.h>
#include <map>

namespace NSGui {

    class Font {

    private: struct GlyphData;
    public:

        GLuint getTexID() const { return texID; }
        GlyphData& getGlyph(char c) { return glyphs[c]; }

    private:
        struct GlyphData {
            float xTextureCoord;
            float yTextureCoord;
            float xMaxTextureCoord;
            float yMaxTextureCoord;
            float xOffset;
            float yOffset;
            float xSize;
            float ySize;
            float xAdvance;
        };

        std::map<char, GlyphData> glyphs;
        GLuint texID;

    };

}

#endif // FONT_H_
