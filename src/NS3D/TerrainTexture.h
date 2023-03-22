#ifndef TERRAINTEXTURE_INCLUDED_H
#define TERRAINTEXTURE_INCLUDED_H

#include <GL/glew.h>

namespace NS3D {

    class TerrainTexture {
        public:
            TerrainTexture(GLuint bg, GLuint r, GLuint g, GLuint b) :
                bgTexID(bg), rTexID(r), gTexID(g), bTexID(b) {}

            GLuint getBgTexID() const { return bgTexID; }
            GLuint getRTexID() const { return rTexID; }
            GLuint getGTexID() const { return gTexID; }
            GLuint getBTexID() const { return bTexID; }

        private:
            GLuint bgTexID;
            GLuint rTexID;
            GLuint gTexID;
            GLuint bTexID;
    };

}

#endif
