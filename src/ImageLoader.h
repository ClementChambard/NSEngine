#ifndef IMAGELOADER
#define IMAGELOADER

#include "GLTexture.h"
#include <vector>
#include "./defines.h"

namespace ns {

class ImageLoader {

    public:
        static GLTexture emptyTexture(i32 w, i32 h);//filters
        static GLTexture loadTexture(cstr filename);
        static GLTexture loadImageFile(cstr filename, cstr filename2="", cstr filename3="", cstr filename4="");
        static GLTexture loadImageBuffer(robytes buffer, std::vector<i32> lengths);

        static void freeImageBuffer(bytes buffer);

        static bytes imageToBuffer(cstr filename, i32& w, i32& h, i32& nrChannels);

        static void textureParameters(GLTexture tex, std::vector<GLenum> params, std::vector<GLint> options);
};

}

#endif
