#ifndef FRAMEBUFFER_HEADER_INCLUDED
#define FRAMEBUFFER_HEADER_INCLUDED

#include "./defines.h"

namespace ns {

class FrameBuffer {
    public:
        enum class Type { NONE = 0, DEPTH_TEXTURE, DEPTH_RENDER_BUFFER };

        FrameBuffer(u32 width, u32 height, Type db);
        FrameBuffer(u32 width, u32 height);
        FrameBuffer(FrameBuffer const& fb);
        FrameBuffer(FrameBuffer&& fb);
        ~FrameBuffer();

        FrameBuffer& operator=(FrameBuffer const& fb);
        FrameBuffer& operator=(FrameBuffer&& fb);

        void bind() const { bind_framebuffer(this); }
        void unbind() const { unbind_framebuffer(); }

        void resolve_to_fbo(FrameBuffer* fbo);
        void resolve_to_screen();

        void resize(u32 width, u32 height);

        u32 get_color_texture() const { return m_colorTextureID; }
        u32 get_depth_texture() const { return m_depthTextureID; }

        u32 get_width () const { return m_width ; }
        u32 get_height() const { return m_height; }

        static const FrameBuffer* BOUND_FRAMEBUFFER;
        static void bind_framebuffer(const FrameBuffer* fb);
        static void unbind_framebuffer();

    private:
        u32 m_width;
        u32 m_height;

        u32 m_frameBufferID = 0;
        u32 m_colorTextureID = 0;
        u32 m_depthTextureID = 0;
        u32 m_depthBufferID = 0;
        u32 m_colorBufferID = 0;

        Type m_type;
        bool m_multisample = false;

        void initialise_framebuffer(Type dbType);
        void create_framebuffer();
        void create_texture_attachment();
        void create_depth_texture_attachment();
        void create_multisample_color_attachement();
        void create_depth_buffer_attachment();
};

} // namespace ns

#endif // FRAMEBUFFER_HEADER_INCLUDED
