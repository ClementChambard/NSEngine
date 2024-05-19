#ifndef GLTEXTUREDOTH
#define GLTEXTUREDOTH
#include <GL/glew.h>
#include "./defines.h"

namespace ns {

struct GLTexture {
    GLuint id = 0;
    i32 width = 0;
    i32 height = 0;
    GLuint id2 = 0;
    GLuint id3 = 0;
    GLuint id4 = 0;
    cstr name;
};

struct GLSurface {
    i32 surf = 0;
    GLuint framebuffer = 0;
    f32 ratio = 0.f;
};

}

#endif
