#ifndef IMAGELOADER
#define IMAGELOADER

#include "GLTexture.h"
#include <vector>

namespace NSEngine {

    class ImageLoader {

        public:
            static NSEngine::GLTexture emptyTexture(int w, int h);//filters
            static NSEngine::GLTexture loadTexture(const char* filename);
            static NSEngine::GLTexture loadImageFile(const char* filename, const char* filename2="", const char* filename3="", const char* filename4="");
            static NSEngine::GLTexture loadImageBuffer(const uint8_t* buffer, std::vector<int> lengths);

            static void freeImageBuffer(uint8_t* buffer);

            static uint8_t* imageToBuffer(const char* filename, int& w, int& h, int& nrChannels);

            static void textureParameters(GLTexture tex, std::vector<GLenum> params, std::vector<GLint> options);
    };

}

#endif
