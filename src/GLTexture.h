#ifndef GLTEXTUREDOTH
#define GLTEXTUREDOTH
#include <GL/glew.h>

namespace NSEngine {

    struct GLTexture {
        GLuint id;
        int width;
        int height;
        GLuint id2 = 0;
        GLuint id3 = 0;
        GLuint id4 = 0;
        const char* name;
    };

    struct GLSurface {
        int surf;
        GLuint framebuffer;
        float ratio;
    };

}

#endif
