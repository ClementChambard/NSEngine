#ifndef TEXTURE_HEADER_INCLUDED
#define TEXTURE_HEADER_INCLUDED

#include "./math/types/vec/vec2.hpp"
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
        CLAMP = 0x2900, // GL_CLAMP
        REPEAT = 0x2901, // GL_REPEAT
        MIRROR = 0x8370, // GL_MIRRORED_REPEAT
        UNKNOWN = 0,
    };

    void set_wrapping_u(Wrapping wrap_u);
    void set_wrapping_v(Wrapping wrap_v);
    void set_wrapping(Wrapping wrap_u, Wrapping wrap_v);
    void set_wrapping(Wrapping wrap_both);

    enum class Filter {
        NEAREST = 0x2600, // GL_NEAREST
        LINEAR = 0x2601, // GL_LINEAR
        UNKNOWN = 0,
    };

    void set_min_filter(Filter min_filter);
    void set_mag_filter(Filter mag_filter);
    void set_min_mag_filter(Filter min_filter, Filter mag_filter);
    void set_min_mag_filter(Filter filter_both);

    static Texture* from_opengl(u32 id, u32 w, u32 h);
    static Texture* as_framebuffer(u32 w, u32 h);

    void use();
    void unuse();
    static void unuse_texture();

    vec2 get_size() const { return vec2(m_width, m_height); }
    u32 get_width() const { return m_width; }
    u32 get_height() const { return m_height; }
    FrameBuffer* get_framebuffer() const { return m_framebuffer; }
    u32 get_opengl_id() const { return m_textureId; }

    vec2 get_uv_at(f32 x, f32 y);
    vec2 get_uv_at(vec2 pos);

private:
    u32 m_textureId = 0; // TODO: Move gl out of API
    u32 m_width = 0;
    u32 m_height = 0;
    Filter m_minFilter = Filter::UNKNOWN;
    Filter m_magFilter = Filter::UNKNOWN;
    Wrapping m_wrappingU = Wrapping::UNKNOWN;
    Wrapping m_wrappingV = Wrapping::UNKNOWN;


    // associated FrameBuffer if the texture is part of a FrameBuffer attachment
    FrameBuffer* m_framebuffer = nullptr;
};

} // namespace NSEngine

#endif // TEXTURE_HEADER_INCLUDED
