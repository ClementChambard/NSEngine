#include "./Texture.hpp"
#include "./FrameBuffer.h"
#include "./stb_image.h"

namespace NSEngine {

Texture* Texture::fromOpenGL(GLuint id, uint32_t w, uint32_t h) {
    Texture* t = new Texture();
    t->m_framebuffer = nullptr;
    t->m_textureId = id;
    t->m_width = w;
    t->m_height = h;
    return t;
}

Texture* Texture::asFramebuffer(uint32_t w, uint32_t h) {
    FrameBuffer* fb = new FrameBuffer(w, h, FrameBuffer::DEPTH_RENDER_BUFFER);
    Texture* t = new Texture();
    t->m_framebuffer = fb;
    t->m_height = fb->getHeight();
    t->m_width = fb->getWidth();
    t->m_textureId = fb->getColorTexture();
    return t;
}

Texture::Texture(const char* filename) {
    int nrChannels;
    int width;
    int height;
    unsigned char *data = stbi_load(filename,
        &width, &height, &nrChannels, 0);
    m_width = width;
    m_height = height;

    glGenTextures(1, &m_textureId);

    use();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
        m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    setWrapping(Wrapping::REPEAT);
    setMinMagFilter(Filter::NEAREST);
    unuse();

    stbi_image_free(data);

    m_framebuffer = nullptr;
}

Texture::Texture(uint32_t width, uint32_t height, uint8_t* data) {
    m_width = width;
    m_height = height;
    glGenTextures(1, &m_textureId);
    use();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
        m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    setWrapping(Wrapping::REPEAT);
    setMinMagFilter(Filter::NEAREST);
    unuse();
    m_framebuffer = nullptr;
}

Texture::~Texture() {
    unuse();
    if (m_framebuffer)
        delete m_framebuffer;
    else if (m_textureId)
        glDeleteTextures(1, &m_textureId);
}

Texture::Texture(Texture&& other) {
    m_width = other.m_width;
    m_height = other.m_height;
    m_textureId = other.m_textureId;
    m_framebuffer = other.m_framebuffer;
    other.m_width = 0;
    other.m_height = 0;
    other.m_framebuffer = nullptr;
    other.m_textureId = 0;
}

Texture& Texture::operator=(Texture&& other) {
    m_width = other.m_width;
    m_height = other.m_height;
    m_textureId = other.m_textureId;
    m_framebuffer = other.m_framebuffer;
    other.m_width = 0;
    other.m_height = 0;
    other.m_framebuffer = nullptr;
    other.m_textureId = 0;
    return *this;
}

void Texture::setWrappingU(Wrapping wrapU) {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                    static_cast<GLenum>(wrapU));
}

void Texture::setWrappingV(Wrapping wrapV) {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                    static_cast<GLenum>(wrapV));
}

void Texture::setWrapping(Wrapping wrapU, Wrapping wrapV) {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                    static_cast<GLenum>(wrapU));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                    static_cast<GLenum>(wrapV));
}

void Texture::setWrapping(Wrapping wrapBoth) {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                    static_cast<GLenum>(wrapBoth));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                    static_cast<GLenum>(wrapBoth));
}

void Texture::setMagFilter(Filter magFilter)  {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                    static_cast<GLenum>(magFilter));
}

void Texture::setMinFilter(Filter minFilter) {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    static_cast<GLenum>(minFilter));
}

void Texture::setMinMagFilter(Filter minFilter, Filter magFilter) {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    static_cast<GLenum>(minFilter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                    static_cast<GLenum>(magFilter));
}

void Texture::setMinMagFilter(Filter filterBoth) {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    static_cast<GLenum>(filterBoth));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                    static_cast<GLenum>(filterBoth));
}

void Texture::use() {
    glBindTexture(GL_TEXTURE_2D, m_textureId);
}

void Texture::unuse() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::unuseTexture() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

glm::vec2 Texture::getUvAt(float x, float y) {
    return {
        x / static_cast<float>(m_width),
        y / static_cast<float>(m_height)
    };
}

glm::vec2 Texture::getUvAt(glm::vec2 pos) {
    return {
        pos.x / static_cast<float>(m_width),
        pos.y / static_cast<float>(m_height)
    };
}

} // namespace NSEngine
