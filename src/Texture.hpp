#ifndef TEXTURE_INCLUDED_H
#define TEXTURE_INCLUDED_H

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace NSEngine {

class FrameBuffer;

class Texture {
public:
    Texture() {}
    Texture(uint32_t width, uint32_t height, uint8_t* data);
    explicit Texture(const char* name);
    ~Texture();
    Texture(Texture const& other) = delete;
    Texture(Texture&& other);
    Texture& operator=(Texture const& other) = delete;
    Texture& operator=(Texture&& other);

    enum class Wrapping {
        REPEAT = GL_REPEAT,
        CLAMP = GL_CLAMP,
        MIRROR = GL_MIRRORED_REPEAT,
    };

    static void setWrappingU(Wrapping wrapU);
    static void setWrappingV(Wrapping wrapV);
    static void setWrapping(Wrapping wrapU, Wrapping wrapV);
    static void setWrapping(Wrapping wrapBoth);

    enum class Filter {
        NEAREST = GL_NEAREST,
        LINEAR = GL_LINEAR,
    };

    static void setMinFilter(Filter minFilter);
    static void setMagFilter(Filter magFilter);
    static void setMinMagFilter(Filter minFilter, Filter magFilter);
    static void setMinMagFilter(Filter filterBoth);

    static Texture* fromOpenGL(GLuint id, uint32_t w, uint32_t h);
    static Texture* asFramebuffer(uint32_t w, uint32_t h);

    void use();
    void unuse();
    static void unuseTexture();

    glm::vec2 getSize() const { return {m_width, m_height}; }
    uint32_t getWidth() const { return m_width; }
    uint32_t getHeight() const { return m_height; }
    FrameBuffer* getFramebuffer() const { return m_framebuffer; }
    GLuint getOpenglId() const { return m_textureId; }

    glm::vec2 getUvAt(float x, float y);
    glm::vec2 getUvAt(glm::vec2 pos);

private:
    GLuint m_textureId = 0;
    uint32_t m_width = 0;
    uint32_t m_height = 0;

    // associated FrameBuffer if the texture is part of a FrameBuffer attachment
    FrameBuffer* m_framebuffer = nullptr;
};

} // namespace NSEngine

#endif // !TEXTURE_INCLUDED_H
