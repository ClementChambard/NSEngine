#include "./Texture.hpp"
#include "./FrameBuffer.h"
#include "./memory.h"

#include <GL/glew.h>

#define STB_IMAGE_IMPLEMENTATION
#include "./vendor/stb_image.h"


namespace ns {

Texture* Texture::from_opengl(u32 id, u32 w, u32 h) {
    Texture* t = alloc<Texture, MemTag::TEXTURE>();
    t->m_framebuffer = nullptr;
    t->m_textureId = id;
    t->m_width = w;
    t->m_height = h;
    return t;
}

Texture* Texture::as_framebuffer(u32 w, u32 h) {
    FrameBuffer *fb = construct<FrameBuffer, MemTag::TEXTURE>(w, h, FrameBuffer::Type::DEPTH_RENDER_BUFFER);
    Texture* t = construct<Texture, MemTag::TEXTURE>();
    t->m_framebuffer = fb;
    t->m_height = fb->get_height();
    t->m_width = fb->get_width();
    t->m_textureId = fb->get_color_texture();
    return t;
}

Texture::Texture(cstr filename) {
    i32 nrChannels;
    i32 width;
    i32 height;
    bytes data = stbi_load(filename,
        &width, &height, &nrChannels, 0);
    m_width = width;
    m_height = height;

    glCreateTextures(GL_TEXTURE_2D, 1, &m_textureId);
    glTextureStorage2D(m_textureId, 1, GL_RGBA8, m_width, m_height);
    set_data(data);

    set_wrapping(Wrapping::REPEAT);
    set_min_mag_filter(Filter::NEAREST);

    stbi_image_free(data);

    m_framebuffer = nullptr;
}

Texture::Texture(u32 width, u32 height, bytes data) {
    m_width = width;
    m_height = height;
    glCreateTextures(GL_TEXTURE_2D, 1, &m_textureId);
    glTextureStorage2D(m_textureId, 1, GL_RGBA8, m_width, m_height);
    set_data(data);

    set_wrapping(Wrapping::REPEAT);
    set_min_mag_filter(Filter::NEAREST);
    m_framebuffer = nullptr;
}

Texture::~Texture() {
    if (m_framebuffer)
        ns::destroy<MemTag::TEXTURE>(m_framebuffer);
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

void Texture::set_wrapping_u(Wrapping wrapU) {
    if (m_wrappingU != wrapU) {
        m_wrappingU = wrapU;
        glTextureParameteri(m_textureId, GL_TEXTURE_WRAP_S, static_cast<GLenum>(wrapU));
    }
}

void Texture::set_wrapping_v(Wrapping wrapV) {
    if (m_wrappingV != wrapV) {
        m_wrappingV = wrapV;
        glTextureParameteri(m_textureId, GL_TEXTURE_WRAP_T, static_cast<GLenum>(wrapV));
    }
}

void Texture::set_wrapping(Wrapping wrapU, Wrapping wrapV) {
    set_wrapping_u(wrapU);
    set_wrapping_v(wrapV);
}

void Texture::set_wrapping(Wrapping wrapBoth) {
    set_wrapping(wrapBoth, wrapBoth);
}

void Texture::set_mag_filter(Filter magFilter)  {
    if (m_magFilter != magFilter) {
        m_magFilter = magFilter;
        glTextureParameteri(m_textureId, GL_TEXTURE_MAG_FILTER, static_cast<GLenum>(magFilter));
    }
}

void Texture::set_min_filter(Filter minFilter) {
    if (m_minFilter != minFilter) {
        m_minFilter = minFilter;
        glTextureParameteri(m_textureId, GL_TEXTURE_MIN_FILTER, static_cast<GLenum>(minFilter));
    }
}

void Texture::set_min_mag_filter(Filter minFilter, Filter magFilter) {
    set_min_filter(minFilter);
    set_mag_filter(magFilter);
}

void Texture::set_min_mag_filter(Filter filterBoth) {
    set_min_mag_filter(filterBoth, filterBoth);
}

void Texture::set_data(u32 offset_x, u32 offset_y, u32 width, u32 height, bytes data) {
    glTextureSubImage2D(m_textureId, 0, offset_x, offset_y, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
}

void Texture::use(u32 unit) {
    glBindTextureUnit(unit, m_textureId);
}

// deprecated
void Texture::unuse() {}
void Texture::unuse_texture() {}

vec2 Texture::get_uv_at(f32 x, f32 y) {
    return {
        x / static_cast<float>(m_width),
        y / static_cast<float>(m_height)
    };
}

vec2 Texture::get_uv_at(vec2 pos) {
    return {
        pos.x / static_cast<float>(m_width),
        pos.y / static_cast<float>(m_height)
    };
}

} // namespace NSEngine
