#ifndef SPRITEBATCH
#define SPRITEBATCH

#include <GL/glew.h>
#include <cstddef>
#include <glm/glm.hpp>
#include <vector>
#include "vertex.h"

namespace NSEngine {

    enum UniformType {
        FLOAT1, FLOAT2, FLOAT3, FLOAT4,
        INT1, INT2, INT3, INT4,
        UINT1, UINT2, UINT3, UINT4,
        MAT2, MAT3, MAT4, MAT23, MAT32,
        MAT24, MAT42, MAT34, MAT43
    };

    struct SpriteBatchData {
        struct uniformChange {
            GLint uniformID;
            void* data;
            UniformType type;
            void Apply();
        };
        unsigned int place = 0;
        GLuint texture = 0;
        GLubyte blendmode = 0;
        bool is3d = false;
        uint16_t shader = 0;
        std::vector<uniformChange> uniformChanges;
        uint8_t whatChanges = 0;
    };

    class Glyph {

        public:
            Glyph(GLuint tex, Vertex tl, Vertex tr, Vertex br, Vertex bl, int bm = 0, bool i3d = false) : texture(tex), blendmode(bm), is3d(i3d), topLeft(tl), topRight(tr), bottomRight(br), bottomLeft(bl) {};

            GLuint texture;
            GLubyte blendmode = 0;
            bool is3d = false;
            Vertex topLeft;
            Vertex topRight;
            Vertex bottomRight;
            Vertex bottomLeft;
    };

    class RenderBatch {
        public:
            RenderBatch(GLuint ofs, GLuint nb, GLuint tex, int bm, bool i3d = false) : offset(ofs), numVertices(nb), texture(tex), blendmode(bm), is3d(i3d) {}
            GLuint offset;
            GLuint numVertices;
            GLuint texture;
            GLubyte blendmode;
            bool is3d;
    };

    class SpriteBatch {
        public:
            SpriteBatch();
            SpriteBatch(bool sort) : SpriteBatch() { depthSort = sort; }
            ~SpriteBatch();

            SpriteBatch* Init();
            void Dispose();

            void begin();
            void end(bool staticdraw = false);
            void clear();

            void draw(GLuint texture, Vertex tl, Vertex tr, Vertex br, Vertex bl, int blendmode = 0, bool is3d = false);

            void renderBatch();

            std::size_t size() const { return glyphs.size(); }

            bool depthTest = false;
            bool is_static = false;
        private:
            bool depthSort = false;
            GLuint vbo;
            std::vector<Glyph*> glyphsptr;
            std::vector<Glyph> glyphs;
            std::vector<RenderBatch> renderBatches;
            std::vector<SpriteBatchData> changes;
            void createRenderBatches(bool staticdraw);
            void bindBuffer();
            void sortGlyphs(bool sort = true);
    };

}

#endif
