#include "WaterFrameBuffers.h"
#include "FrameBuffer.h"
// #include "../NSEngine.hpp"

namespace ns::n3d {

ns::FrameBuffer* WaterFrameBuffers::reflection_frame_buffer;
ns::FrameBuffer* WaterFrameBuffers::refraction_frame_buffer;

void WaterFrameBuffers::init()
{
    reflection_frame_buffer = new ns::FrameBuffer(REFLECTION_WIDTH, REFLECTION_HEIGHT, ns::FrameBuffer::Type::DEPTH_RENDER_BUFFER);
    refraction_frame_buffer = new ns::FrameBuffer(REFLECTION_WIDTH, REFLECTION_HEIGHT, ns::FrameBuffer::Type::DEPTH_TEXTURE);
}

void WaterFrameBuffers::cleanup()
{
    delete reflection_frame_buffer;
    delete refraction_frame_buffer;
}

void WaterFrameBuffers::bind_reflection_frame_buffer()
{
    reflection_frame_buffer->bind();
}

void WaterFrameBuffers::bind_refraction_frame_buffer()
{
    refraction_frame_buffer->bind();
}

void WaterFrameBuffers::unbind_current_frame_buffer()
{
    FrameBuffer::unbind_framebuffer();
}

}
