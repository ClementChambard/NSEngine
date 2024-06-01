#ifndef FRAMEBUFFER_INCLUDED_H
#define FRAMEBUFFER_INCLUDED_H

#include <GL/glew.h>
#include "./defines.h"

namespace ns {

class FrameBuffer {
    public:
        enum type { NONE = 0, DEPTH_TEXTURE, DEPTH_RENDER_BUFFER };

        FrameBuffer(u32 width, u32 height, type db);
        FrameBuffer(u32 width, u32 height);
        FrameBuffer(FrameBuffer const& fb);
        FrameBuffer(FrameBuffer&& fb);
        ~FrameBuffer();

        FrameBuffer& operator=(FrameBuffer const& fb);
        FrameBuffer& operator=(FrameBuffer&& fb);

        void bind() const { bindFramebuffer(this); }
        void unbind() const { unbindFramebuffer(); }

        void resolveToFBO(FrameBuffer* fbo);
        void resolveToScreen();

        void resize(u32 width, u32 height);

        GLuint getColorTexture() const { return m_colorTextureID; }
        GLuint getDepthTexture() const { return m_depthTextureID; }

        u32 getWidth () const { return m_width ; }
        u32 getHeight() const { return m_height; }

        static const FrameBuffer* BOUND_FRAMEBUFFER;
        static void bindFramebuffer(const FrameBuffer* fb);
        static void unbindFramebuffer();

    private:
        u32 m_width;
        u32 m_height;

        GLuint m_frameBufferID = 0;
        GLuint m_colorTextureID = 0;
        GLuint m_depthTextureID = 0;
        GLuint m_depthBufferID = 0;
        GLuint m_colorBufferID = 0;

        type m_type;
        bool m_multisample = false;

        void initialiseFrameBuffer(type dbType);
        void createFrameBuffer();
        void createTextureAttachment();
        void createDepthTextureAttachment();
        void createMultisampleColorAttachement();
        void createDepthBufferAttachment();
};

}

#endif
