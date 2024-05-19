#ifndef TEXTURE_INCLUDED_H
#define TEXTURE_INCLUDED_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "./defines.h"

namespace ns {

class FrameBuffer;

class Texture {
public:
    Texture() {}
    Texture(u32 width, u32 height, bytes data);
    explicit Texture(cstr name);
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

    static Texture* fromOpenGL(GLuint id, u32 w, u32 h);
    static Texture* asFramebuffer(u32 w, u32 h);

    void use();
    void unuse();
    static void unuseTexture();

    glm::vec2 getSize() const { return {m_width, m_height}; }
    u32 getWidth() const { return m_width; }
    u32 getHeight() const { return m_height; }
    FrameBuffer* getFramebuffer() const { return m_framebuffer; }
    GLuint getOpenglId() const { return m_textureId; }

    glm::vec2 getUvAt(f32 x, f32 y);
    glm::vec2 getUvAt(glm::vec2 pos);

private:
    GLuint m_textureId = 0;
    u32 m_width = 0;
    u32 m_height = 0;

    // associated FrameBuffer if the texture is part of a FrameBuffer attachment
    FrameBuffer* m_framebuffer = nullptr;
};

} // namespace NSEngine

#endif // !TEXTURE_INCLUDED_H
