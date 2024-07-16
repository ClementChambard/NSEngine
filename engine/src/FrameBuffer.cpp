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
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo->m_frameBufferID);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_frameBufferID);
    glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, fbo->m_width, fbo->m_height,
            GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    this->unbind();
}

void FrameBuffer::resolve_to_screen()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_frameBufferID);
    glDrawBuffer(GL_BACK);
    auto wincfg = get_main_window()->get_config();
    glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, wincfg.width, wincfg.height,
            GL_COLOR_BUFFER_BIT, GL_NEAREST);
    this->unbind();
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
    unbind();
}

void FrameBuffer::create_framebuffer()
{
    glGenFramebuffers(1, &m_frameBufferID);
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferID);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
}

void FrameBuffer::create_texture_attachment()
{
    glGenTextures(1, &m_colorTextureID);
    glBindTexture(GL_TEXTURE_2D, m_colorTextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_colorTextureID, 0);
}

void FrameBuffer::create_depth_texture_attachment()
{
    glGenTextures(1, &m_depthTextureID);
    glBindTexture(GL_TEXTURE_2D, m_depthTextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_depthTextureID, 0);
}

void FrameBuffer::create_multisample_color_attachement()
{
    glGenRenderbuffers(1, &m_colorBufferID);
    glBindRenderbuffer(GL_RENDERBUFFER, m_colorBufferID);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_RGBA8, m_width, m_height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_colorBufferID);
}

void FrameBuffer::create_depth_buffer_attachment()
{
    glGenRenderbuffers(1, &m_depthBufferID);
    glBindRenderbuffer(GL_RENDERBUFFER, m_depthBufferID);
    if (!m_multisample)
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, m_width, m_height);
    else
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH_COMPONENT24, m_width, m_height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBufferID);
}

void FrameBuffer::resize(u32 width, u32 height)
{
    this->m_width = width;
    this->m_height = height;
    glBindTexture(GL_TEXTURE_2D, m_colorTextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glBindTexture(GL_TEXTURE_2D, 0);
}

const FrameBuffer* FrameBuffer::BOUND_FRAMEBUFFER = nullptr;

void FrameBuffer::bind_framebuffer(const FrameBuffer* fb) {
    BOUND_FRAMEBUFFER = fb;
    if (!fb) {
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
