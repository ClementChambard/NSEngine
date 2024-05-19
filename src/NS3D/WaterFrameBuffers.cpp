#include "WaterFrameBuffers.h"
#include "../NSEngine.hpp"

namespace NS3D { 

ns::FrameBuffer* WaterFrameBuffers::reflectionFrameBuffer;
ns::FrameBuffer* WaterFrameBuffers::refractionFrameBuffer;

void WaterFrameBuffers::Init()
{
    reflectionFrameBuffer = new ns::FrameBuffer(REFLECTION_WIDTH, REFLECTION_HEIGHT, ns::FrameBuffer::DEPTH_RENDER_BUFFER);
    refractionFrameBuffer = new ns::FrameBuffer(REFLECTION_WIDTH, REFLECTION_HEIGHT, ns::FrameBuffer::DEPTH_TEXTURE);
}

void WaterFrameBuffers::cleanUp()
{
    delete reflectionFrameBuffer;
    delete refractionFrameBuffer;
}

void WaterFrameBuffers::bindReflectionFrameBuffer()
{
    reflectionFrameBuffer->bind();
}

void WaterFrameBuffers::bindRefractionFrameBuffer()
{
    refractionFrameBuffer->bind();
}

void WaterFrameBuffers::unbindCurrentFrameBuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, ns::getInstance()->window().getWindowData().bwidth, ns::getInstance()->window().getWindowData().bheight);
}

}
