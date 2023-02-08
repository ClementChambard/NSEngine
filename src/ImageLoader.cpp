#include "ImageLoader.h"
#include <string>
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ASSERT(x)
#include "stb_image.h"
#include <iostream>
namespace NSEngine {

    GLTexture ImageLoader::emptyTexture(int w, int h)
    {
        GLTexture tex;
        glGenTextures(1,&(tex.id));
        glBindTexture(GL_TEXTURE_2D, tex.id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glBindTexture(GL_TEXTURE_2D,0);
        tex.width = w; tex.height = h;
        return tex;
    }

    uint8_t* ImageLoader::imageToBuffer(const char *filename, int& w, int& h, int& nr_channels)
    {
        return stbi_load(filename, &w, &h, &nr_channels, 0);
    }

    void ImageLoader::freeImageBuffer(uint8_t* buffer)
    {
        stbi_image_free(buffer);
    }

    void ImageLoader::textureParameters(GLTexture tex, std::vector<GLenum> params, std::vector<GLint> options)
    {
        glBindTexture(GL_TEXTURE_2D, tex.id);
        for (size_t i = 0; i < params.size(); i++)
        {
            glTexParameteri(GL_TEXTURE_2D, params[i], options[i]);
        }
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    GLTexture ImageLoader::loadTexture(const char* filename)
    {
        GLTexture texture = {};

        int nrChannels;
        unsigned char *data = stbi_load(filename, &(texture.width), &(texture.height), &nrChannels, 0);

        glGenTextures(1, &(texture.id));

        glBindTexture(GL_TEXTURE_2D, texture.id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -.4);
        float amount = 4.f;
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, amount);

        glBindTexture(GL_TEXTURE_2D, 0);

        stbi_image_free(data);
        return texture;
    }

    GLTexture ImageLoader::loadImageFile(const char* filename, const char* filename2, const char* filename3, const char* filename4)
    {
        GLTexture texture = {};

        int nrChannels;
        unsigned char *data = stbi_load(filename, &(texture.width), &(texture.height), &nrChannels, 0);

        glGenTextures(1, &(texture.id));

        glBindTexture(GL_TEXTURE_2D, texture.id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        glBindTexture(GL_TEXTURE_2D, 0);
        stbi_image_free(data);

        int w, h;
        if (std::string(filename2) != "")
        {
            data = stbi_load(filename2, &w, &h, &nrChannels, 0);

            glGenTextures(1, &(texture.id2));

            glBindTexture(GL_TEXTURE_2D, texture.id2);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

            glBindTexture(GL_TEXTURE_2D, 0);

            stbi_image_free(data);
        }
        if (std::string(filename3) != "")
        {
            data = stbi_load(filename3, &w, &h, &nrChannels, 0);

            glGenTextures(1, &(texture.id3));

            glBindTexture(GL_TEXTURE_2D, texture.id3);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

            glBindTexture(GL_TEXTURE_2D, 0);

            stbi_image_free(data);
        }
        if (std::string(filename4) != "")
        {
            data = stbi_load(filename4, &w, &h, &nrChannels, 0);

            glGenTextures(1, &(texture.id4));

            glBindTexture(GL_TEXTURE_2D, texture.id4);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

            glBindTexture(GL_TEXTURE_2D, 0);

            stbi_image_free(data);
        }
        return texture;
    }

    GLTexture ImageLoader::loadImageBuffer(const uint8_t* buffer, std::vector<int> lengths)
    {
        GLTexture texture = {};

        int nrChannels;
        unsigned char *data = stbi_load_from_memory(buffer, lengths[0], &(texture.width), &(texture.height), &nrChannels, 0);

        glGenTextures(1, &(texture.id));

        glBindTexture(GL_TEXTURE_2D, texture.id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        glBindTexture(GL_TEXTURE_2D, 0);

        stbi_image_free(data);

        int w, h;
        if (lengths.size()>1)
        {
            data = stbi_load_from_memory(&(buffer[lengths[0]]), lengths[1], &w, &h, &nrChannels, 0);

            glGenTextures(1, &(texture.id2));

            glBindTexture(GL_TEXTURE_2D, texture.id2);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

            glBindTexture(GL_TEXTURE_2D, 0);

            stbi_image_free(data);
        }
        if (lengths.size() > 2)
        {
            data = stbi_load_from_memory(&(buffer[lengths[1]]), lengths[2], &w, &h, &nrChannels, 0);

            glGenTextures(1, &(texture.id3));

            glBindTexture(GL_TEXTURE_2D, texture.id3);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

            glBindTexture(GL_TEXTURE_2D, 0);

            stbi_image_free(data);
        }
        if (lengths.size() > 3)
        {
            data = stbi_load_from_memory(&(buffer[lengths[2]]), lengths[3], &w, &h, &nrChannels, 0);

            glGenTextures(1, &(texture.id4));

            glBindTexture(GL_TEXTURE_2D, texture.id4);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

            glBindTexture(GL_TEXTURE_2D, 0);

            stbi_image_free(data);
        }
        return texture;
    }
}
