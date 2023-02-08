#ifndef FRAMEBUFFER_INCLUDED_H
#define FRAMEBUFFER_INCLUDED_H

#include <GL/glew.h>

namespace NSEngine {

    class FrameBuffer {
        public:
            enum type { NONE = 0, DEPTH_TEXTURE, DEPTH_RENDER_BUFFER };

            FrameBuffer(unsigned int width, unsigned int height, type db);
            FrameBuffer(unsigned int width, unsigned int height);
            FrameBuffer(FrameBuffer const& fb);
            FrameBuffer(FrameBuffer&& fb);
            ~FrameBuffer();

            FrameBuffer& operator=(FrameBuffer const& fb);
            FrameBuffer& operator=(FrameBuffer&& fb);

            void bind();
            void unbind();

            void resolveToFBO(FrameBuffer* fbo);
            void resolveToScreen();

            void resize(unsigned int width, unsigned int height);

            GLuint getColorTexture() const { return m_colorTextureID; }
            GLuint getDepthTexture() const { return m_depthTextureID; }

            unsigned int getWidth () const { return m_width ; }
            unsigned int getHeight() const { return m_height; }

        private:
            unsigned int m_width;
            unsigned int m_height;

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
