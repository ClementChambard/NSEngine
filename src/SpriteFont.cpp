#include "SpriteFont.h"

#include "GLTexture.h"
#include "NSEngine.h"
#include "SpriteBatch.h"
#include "TextureManager.h"
#include "FileOpener.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

int closestPow2(int i) {
    i--;
    int pi = 1;
    while(i > 0) {
        i >>= 1;
        pi <<=1;
    }
    return pi;
}

#define MAX_TEXTURE_RES 4096

NSEngine::font_align halign = NSEngine::fa_left;
NSEngine::font_align valign = NSEngine::fa_top;

namespace NSEngine {

    SpriteFont::SpriteFont() {}

    SpriteFont::SpriteFont(const char* font)
    {
        FileOpener::openFile(font);
        nbofglyphs = FileOpener::readUint8(0);       
        int nbofTexture = FileOpener::readUint8(1+9*nbofglyphs);
        int of = 2+9*nbofglyphs;
        std::string tex1 = FileOpener::readString(of,of);
        std::string tex2 = nbofTexture>1?FileOpener::readString(of+1,of):"";
        std::string tex3 = nbofTexture>2?FileOpener::readString(of+1,of):"";
        std::string tex4 = nbofTexture>3?FileOpener::readString(of+1,of):"";
        texID = TextureManager::RegisterTexture(tex1.c_str(),tex2.c_str(),tex3.c_str(),tex4.c_str());
        glyphs = new CharGlyph[nbofglyphs];       
        int fontHeight = 0;
        for (int i = 0; i < nbofglyphs; i++)
        {
            glyphs[i].character = FileOpener::readInt8(1+9*i);
            int x = FileOpener::readUint16(2+9*i);
            int y = FileOpener::readUint16(4+9*i);
            int w = FileOpener::readUint16(6+9*i) - x;
            int h = FileOpener::readUint16(8+9*i) - y;
            if (h > fontHeight) fontHeight = h;
            glyphs[i].size = {w,h};
            UV uv1, uvS;
            uv1 = TextureManager::GetUVAt(texID,x,y);
            uvS = TextureManager::GetUVAt(texID,w,h);
            glyphs[i].uvRect = {uv1.u,uv1.v,uvS.u,uvS.v};
        }
        fromString = true;
    }

    SpriteFont::SpriteFont(const char* font, int size)
    {
        // Initialize SDL_ttf
        if (!TTF_WasInit()) {
            TTF_Init();
        }
        TTF_Font* f = TTF_OpenFont(font, size);
        if (f == nullptr) {
            fprintf(stderr, "Failed to open TTF font %s\n", font);
            fflush(stderr);
            throw 281;
        }
        fontHeight = TTF_FontHeight(f);
        char regStart = FIRST_PRINTABLE_CHAR;
        char regLength = LAST_PRINTABLE_CHAR - FIRST_PRINTABLE_CHAR + 1;
        int padding = size / 8;

        // First neasure all the regions
        glm::ivec4* glyphRects = new glm::ivec4[regLength];
        int i = 0, advance;
        for (char c = FIRST_PRINTABLE_CHAR; c <= LAST_PRINTABLE_CHAR; c++) {
            TTF_GlyphMetrics(f, c, &glyphRects[i].x, &glyphRects[i].z, &glyphRects[i].y, &glyphRects[i].w, &advance);
            glyphRects[i].z -= glyphRects[i].x;
            glyphRects[i].x = 0;
            glyphRects[i].w -= glyphRects[i].y;
            glyphRects[i].y = 0;
            i++;
        }

        // Find best partitioning of glyphs
        int rows = 1, w, h, bestWidth = 0, bestHeight = 0, area = MAX_TEXTURE_RES * MAX_TEXTURE_RES, bestRows = 0;
        std::vector<int>* bestPartition = nullptr;
        while (rows <= regLength) {
            h = rows * (padding + fontHeight) + padding;
            auto gr = createRows(glyphRects, regLength, rows, padding, w);

            // Desire a power of 2 texture
            w = closestPow2(w);
            h = closestPow2(h);

            // A texture must be feasible
            if (w > MAX_TEXTURE_RES || h > MAX_TEXTURE_RES) {
                rows++;
                delete[] gr;
                continue;
            }

            // Check for minimal area
            if (area >= w * h) {
                if (bestPartition) delete[] bestPartition;
                bestPartition = gr;
                bestWidth = w;
                bestHeight = h;
                bestRows = rows;
                area = bestWidth * bestHeight;
                rows++;
            } else {
                delete[] gr;
                break;
            }
        }

        // Can a bitmap font be made?
        if (!bestPartition) {
            fprintf(stderr, "Failed to Map TTF font %s to texture. Try lowering resolution.\n", font);
            fflush(stderr);
            throw 282;
        }
        // Create the texture
        glGenTextures(1, &texID);
        glBindTexture(GL_TEXTURE_2D, texID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bestWidth, bestHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

        // Now draw all the glyphs
        SDL_Color fg = { 255, 255, 255, 255 };
        int ly = padding;
        for (int ri = 0; ri < bestRows; ri++) {
            int lx = padding;
            for (size_t ci = 0; ci < bestPartition[ri].size(); ci++) {
                int gi = bestPartition[ri][ci];

                SDL_Surface* glyphSurface = TTF_RenderGlyph_Blended(f, (char)(FIRST_PRINTABLE_CHAR + gi), fg);

                // Pre-multiplication occurs here
                unsigned char* sp = (unsigned char*)glyphSurface->pixels;
                int cp = glyphSurface->w * glyphSurface->h * 4;
                for (int i = 0; i < cp; i += 4) {
                    float a = sp[i + 3] / 255.0f;
                    sp[i] = (unsigned char)((float)sp[i] * a);
                    sp[i + 1] = sp[i];
                    sp[i + 2] = sp[i];
                }

                // Save glyph image and update coordinates
                glTexSubImage2D(GL_TEXTURE_2D, 0, lx, bestHeight - ly - 1 - glyphSurface->h, glyphSurface->w, glyphSurface->h, GL_BGRA, GL_UNSIGNED_BYTE, glyphSurface->pixels);
                glyphRects[gi].x = lx;
                glyphRects[gi].y = bestHeight - ly - 1 - glyphSurface->h;//ly;
                glyphRects[gi].z = glyphSurface->w;
                glyphRects[gi].w = glyphSurface->h;

                SDL_FreeSurface(glyphSurface);
                glyphSurface = nullptr;

                lx += glyphRects[gi].z + padding;
            }
            ly += fontHeight + padding;
        }

        // Draw the unsupported glyph
        int rs = padding - 1;
        int* pureWhiteSquare = new int[rs * rs];
        memset(pureWhiteSquare, 0xffffffff, rs * rs * sizeof(int));
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, rs, rs, GL_RGBA, GL_UNSIGNED_BYTE, pureWhiteSquare);
        delete[] pureWhiteSquare;
        pureWhiteSquare = nullptr;

        // Set some texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        // Create spriteBatch glyphs
        glyphs = new CharGlyph[regLength + 1];
        for (i = 0; i < regLength; i++) {
            glyphs[i].character = (char)(FIRST_PRINTABLE_CHAR + i);
            glyphs[i].size = glm::vec2(glyphRects[i].z, glyphRects[i].w);
            glyphs[i].uvRect = glm::vec4(
                (float)glyphRects[i].x / (float)bestWidth,
                (float)glyphRects[i].y / (float)bestHeight,
                (float)glyphRects[i].z / (float)bestWidth,
                (float)glyphRects[i].w / (float)bestHeight
                );
        }
        glyphs[regLength].character = ' ';
        glyphs[regLength].size = glyphs[0].size;
        glyphs[regLength].uvRect = glm::vec4(0, 0, (float)rs / (float)bestWidth, (float)rs / (float)bestHeight);

        glBindTexture(GL_TEXTURE_2D, 0);
        delete[] glyphRects;
        delete[] bestPartition;
        TTF_CloseFont(f);
        int a = TextureManager::AddTexture(texID, w, h);
        texID = a;
    }

    SpriteFont::SpriteFont(int texID, int width, int height, const char* chars, int numberofchars, int offsetX, int offsetY, int padding)
    {
        fromString = true;
        this->texID = texID;
        fontHeight = height;
        int tw, th;
        nbofglyphs = numberofchars;
        this->padding = padding;
        TextureManager::GetTextureSize(texID, tw, th);
        glyphs = new CharGlyph[numberofchars];
        int pixelx = offsetX, pixely = offsetY;
        UV uvsize = TextureManager::GetUVAt(texID, width, height);
        if (pixelx + width > tw || pixely + height > th)
        {
            error("Can't create spritefont with that texture");
            return;
        }
        for (int i = 0; i < numberofchars; i++)
        {
            glyphs[i].character = chars[i];
            glyphs[i].size = {width, height};
            UV uv1 = TextureManager::GetUVAt(texID, pixelx, pixely);
            glyphs[i].uvRect = {uv1.u, uv1.v, uvsize.u, uvsize.v};
            pixelx += width;
            if (pixelx + width > tw)
            {
                pixelx = offsetX;
                pixely += height;
                if (pixely + height > th)
                {
                    error("Character out of bond on texture");
                    return;
                }
            }
        }
    }


    SpriteFont::~SpriteFont() {
        if (texID != -1) {
            glDeleteTextures(1, &texID);
            texID = -1;
        }
        if (glyphs) {
            delete[] glyphs;
            glyphs = nullptr;
        }
    }

    std::vector<int>* SpriteFont::createRows(glm::ivec4* rects, int rectsLength, int r, int padding, int& w) {
        // Blank initialize
        std::vector<int>* l = new std::vector<int>[r]();
        int* cw = new int[r]();
        for (int i = 0; i < r; i++) {
            cw[i] = padding;
        }

        // Loop through all glyphs
        for (int i = 0; i < rectsLength; i++) {
            // Find row for placement
            int ri = 0;
            for (int rii = 1; rii < r; rii++)
            if (cw[rii] < cw[ri]) ri = rii;

            // Add width to that row
            cw[ri] += rects[i].z + padding;

            // Add glyph to the row list
            l[ri].push_back(i);
        }

        // Find the max width
        w = 0;
        for (int i = 0; i < r; i++) {
            if (cw[i] > w) w = cw[i];
        }
        delete[] cw;
        return l;
    }

    glm::vec2 SpriteFont::measure(const char* s) {
        glm::vec2 size(0, fontHeight);
        float cw = 0;
        for (int si = 0; s[si] != 0; si++) {
            char c = s[si];
            if (s[si] == '\n') {
                size.y += fontHeight;
                if (size.x < cw)
                    size.x = cw;
                cw = 0;
            } else {
                // Check for correct glyph
                int gi;
                if (fromString) 
                {
                    gi = -1;
                    for (int i = 0; i < nbofglyphs; i++) if (glyphs[i].character == c) gi = i; 
                }
                else
                {
                    gi = c - FIRST_PRINTABLE_CHAR;
                    if (gi < 0 || gi >= 1+LAST_PRINTABLE_CHAR-FIRST_PRINTABLE_CHAR)
                        gi = 1+LAST_PRINTABLE_CHAR-FIRST_PRINTABLE_CHAR;
                }
                cw += glyphs[gi].size.x;
            }
        }
        if (size.x < cw)
            size.x = cw;
        return size;
    }

    void SpriteFont::draw(const char* s, glm::vec2 position, glm::vec2 scaling, float depth, Color tint) {
        glm::vec2 tp = position;
        // Apply justification
        glm::vec2 size = measure(s) * scaling;
        if (halign == fa_center) { tp.x -= size.x / 2; }
        else if (halign == fa_right) { tp.x -= size.x; }
        if (valign == fa_center) { tp.y -= size.y / 2; }
        else if (valign == fa_top) { tp.y -= size.y; }

        for (int si = 0; s[si] != 0; si++) {
            char c = s[si];
            if (s[si] == '\n') {
                tp.y += fontHeight * scaling.y;
                tp.x = position.x;
            } else {
                // Check for correct glyph
                int gi = c - FIRST_PRINTABLE_CHAR;// - LAST_PRINTABLE_CHAR;
                if (fromString)
                {
                    gi = -1;
                    for (int i = 0; i < nbofglyphs; i++) if (glyphs[i].character == c) gi = i; 
                }
                else
                {
                    int gi = c - FIRST_PRINTABLE_CHAR;// - LAST_PRINTABLE_CHAR;
                    if (gi < 0 || gi >= (1 + LAST_PRINTABLE_CHAR - FIRST_PRINTABLE_CHAR))
                        gi = 1 + LAST_PRINTABLE_CHAR - FIRST_PRINTABLE_CHAR;
                }
                if (gi != -1) {
                    glm::vec2 gs = glyphs[gi].size * scaling;
                    if (c!=' ')
                    engineData::layers[engineData::targetLayer]->getBatch()->draw(texID,
                        {{tp.x,tp.y,depth},           tint, {glyphs[gi].uvRect.x,glyphs[gi].uvRect.y+glyphs[gi].uvRect.w}},
                        {{tp.x+gs.x,tp.y,depth},      tint, {glyphs[gi].uvRect.x+glyphs[gi].uvRect.z,glyphs[gi].uvRect.y+glyphs[gi].uvRect.w}},
                        {{tp.x+gs.x,tp.y+gs.y,depth}, tint, {glyphs[gi].uvRect.x+glyphs[gi].uvRect.z,glyphs[gi].uvRect.y}},
                        {{tp.x,tp.y+gs.y,depth},      tint, {glyphs[gi].uvRect.x,glyphs[gi].uvRect.y}}
                        );
                }
                tp.x += (glyphs[gi].size.x + padding) * scaling.x;
            }
        }
    }

    void draw_set_halign(font_align fa) { halign = fa; }
    void draw_set_valign(font_align fa) { valign = fa; }

}
