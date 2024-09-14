#ifndef WATERFRAMEBUFFERS_HEADER_INCLUDED
#define WATERFRAMEBUFFERS_HEADER_INCLUDED

#include <FrameBuffer.h>
#include <defines.h>

namespace ns::n3d {

class WaterFrameBuffers {
    public:
        static void init();
        static void cleanup();

        static void bind_reflection_frame_buffer();
        static void bind_refraction_frame_buffer();
        static void unbind_current_frame_buffer();

        static u32 get_reflection_texture() { return reflection_frame_buffer->get_color_texture(); }
        static u32 get_refraction_texture() { return refraction_frame_buffer->get_color_texture(); }
        static u32 get_refraction_depth_texture() { return refraction_frame_buffer->get_depth_texture(); }

    protected:
        static constexpr i32 REFLECTION_WIDTH = 320;
        static constexpr i32 REFRACTION_WIDTH = 1280;

    private:
        static const i32 REFLECTION_HEIGHT = 180;
        static const i32 REFRACTION_HEIGHT = 720;

        static ns::FrameBuffer* reflection_frame_buffer;
        static ns::FrameBuffer* refraction_frame_buffer;

};

}

#endif // WATERFRAMEBUFFERS_HEADER_INCLUDED
