#ifndef GLTEXTUREDOTH
#define GLTEXTUREDOTH
#include <GL/glew.h>

namespace NSEngine {

    struct GLTexture {
        GLuint id = 0;
        int width = 0;
        int height = 0;
        GLuint id2 = 0;
        GLuint id3 = 0;
        GLuint id4 = 0;
        const char* name;
    };

    struct GLSurface {
        int surf = 0;
        GLuint framebuffer = 0;
        float ratio = 0.f;
    };

}

#endif
