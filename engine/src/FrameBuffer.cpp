#include "./FrameBuffer.h"
#include "./NSEngine.hpp"

#include <GL/glew.h>

namespace ns {

FrameBuffer::FrameBuffer(u32 width, u32 height, Type db)
{
    m_width = width;
    m_height = height;
    initialise_framebuffer(db);
}

FrameBuffer::FrameBuffer(u32 width, u32 height)
{
    m_width = width;
    m_height = height;
    m_multisample = true;
    initialise_framebuffer(Type::DEPTH_RENDER_BUFFER);
}

FrameBuffer::FrameBuffer(FrameBuffer const& fb)
{
    m_width = fb.m_width;
    m_height = fb.m_height;
    m_multisample = fb.m_multisample;
    initialise_framebuffer(fb.m_type);
}

FrameBuffer::FrameBuffer(FrameBuffer&& fb)
{
    m_width = fb.m_width;
    m_height = fb.m_height;
    m_multisample = fb.m_multisample;
    m_frameBufferID = fb.m_frameBufferID;
    m_colorTextureID = fb.m_colorTextureID;
    m_depthTextureID = fb.m_depthTextureID;
    m_depthBufferID = fb.m_depthBufferID;
    m_colorBufferID = fb.m_colorBufferID;
    m_type = fb.m_type;
    fb.m_width = 0;
    fb.m_height = 0;
    fb.m_multisample = 0;
    fb.m_frameBufferID = 0;
    fb.m_colorTextureID = 0;
    fb.m_colorBufferID = 0;
    fb.m_depthBufferID = 0;
    fb.m_depthTextureID = 0;
}

FrameBuffer::~FrameBuffer()
{
    glDeleteFramebuffers(1, &m_frameBufferID);
    glDeleteTextures(1, &m_colorTextureID);
    glDeleteTextures(1, &m_depthTextureID);
    glDeleteRenderbuffers(1, &m_depthBufferID);
    glDeleteRenderbuffers(1, &m_colorBufferID);
}

FrameBuffer& FrameBuffer::operator=(FrameBuffer const& fb)
{
    m_width = fb.m_width;
    m_height = fb.m_height;
    m_multisample = fb.m_multisample;
    initialise_framebuffer(fb.m_type);
    return *this;
}

FrameBuffer& FrameBuffer::operator=(FrameBuffer&& fb)
{
    m_width = fb.m_width;
    m_height = fb.m_height;
    m_multisample = fb.m_multisample;
    m_frameBufferID = fb.m_frameBufferID;
    m_colorTextureID = fb.m_colorTextureID;
    m_depthTextureID = fb.m_depthTextureID;
    m_depthBufferID = fb.m_depthBufferID;
    m_colorBufferID = fb.m_colorBufferID;
    m_type = fb.m_type;
    fb.m_width = 0;
    fb.m_height = 0;
    fb.m_multisample = 0;
    fb.m_frameBufferID = 0;
    fb.m_colorTextureID = 0;
    fb.m_colorBufferID = 0;
    fb.m_depthBufferID = 0;
    fb.m_depthTextureID = 0;
    return *this;
}

void FrameBuffer::resolve_to_fbo(FrameBuffer* fbo)
{
    glBlitNamedFramebuffer(m_frameBufferID, fbo->m_frameBufferID, 0, 0, m_width, m_height, 0, 0, fbo->m_width, fbo->m_height, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);
}

void FrameBuffer::resolve_to_screen()
{
    auto wincfg = get_main_window()->get_config();
    glBlitNamedFramebuffer(m_frameBufferID, 0, 0, 0, m_width, m_height, 0, 0, wincfg.width, wincfg.height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
}

void FrameBuffer::initialise_framebuffer(Type dbType)
{
    m_type = dbType;
    create_framebuffer();
    if (!m_multisample)
        create_texture_attachment();
    else
        create_multisample_color_attachement();
    if (dbType == Type::DEPTH_RENDER_BUFFER)
        create_depth_buffer_attachment();
    else if (dbType == Type::DEPTH_TEXTURE)
        create_depth_texture_attachment();
}

void FrameBuffer::create_framebuffer()
{
    glCreateFramebuffers(1, &m_frameBufferID);
    glNamedFramebufferDrawBuffer(m_frameBufferID, GL_COLOR_ATTACHMENT0);
}

void FrameBuffer::create_texture_attachment()
{
    glCreateTextures(GL_TEXTURE_2D, 1, &m_colorTextureID);
    glTextureStorage2D(m_colorTextureID, 1, GL_RGBA8, m_width, m_height);
    glTextureParameteri(m_colorTextureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(m_colorTextureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_colorTextureID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(m_colorTextureID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glNamedFramebufferTexture(m_frameBufferID, GL_COLOR_ATTACHMENT0, m_colorTextureID, 0);
}

void FrameBuffer::create_depth_texture_attachment()
{
    glCreateTextures(GL_TEXTURE_2D, 1, &m_depthTextureID);
    glTextureStorage2D(m_depthTextureID, 1, GL_DEPTH_COMPONENT24, m_width, m_height);
    glTextureParameteri(m_depthTextureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(m_depthTextureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glNamedFramebufferTexture(m_frameBufferID, GL_DEPTH_ATTACHMENT, m_depthTextureID, 0);
}

void FrameBuffer::create_multisample_color_attachement()
{
    glCreateRenderbuffers(1, &m_colorBufferID);
    glNamedRenderbufferStorageMultisample(m_colorBufferID, 4, GL_RGBA8, m_width, m_height);
    glNamedFramebufferRenderbuffer(m_frameBufferID, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_colorBufferID);
}

void FrameBuffer::create_depth_buffer_attachment()
{
    glCreateRenderbuffers(1, &m_depthBufferID);
    if (!m_multisample)
        glNamedRenderbufferStorage(m_depthBufferID, GL_DEPTH_COMPONENT24, m_width, m_height);
    else
        glNamedRenderbufferStorageMultisample(m_depthBufferID, 4, GL_DEPTH_COMPONENT24, m_width, m_height);
    glNamedFramebufferRenderbuffer(m_frameBufferID, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBufferID);
}

void FrameBuffer::resize(u32 width, u32 height)
{
    m_width = width;
    m_height = height;
    glTextureStorage2D(m_colorTextureID, 1, GL_RGBA8, width, height);
}

const FrameBuffer* FrameBuffer::BOUND_FRAMEBUFFER = nullptr;

void FrameBuffer::bind_framebuffer(const FrameBuffer* fb) {
    BOUND_FRAMEBUFFER = fb;
    if (fb == nullptr) {
        get_main_window()->bind_as_render_target();
        return;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, fb->m_frameBufferID);
    glViewport(0, 0, fb->m_width, fb->m_height);
}

void FrameBuffer::unbind_framebuffer() {
    BOUND_FRAMEBUFFER = nullptr;
    get_main_window()->bind_as_render_target();
}

}
