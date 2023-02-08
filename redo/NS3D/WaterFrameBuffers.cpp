#include "WaterFrameBuffers.h"
#include "../NSEngine.h"

namespace NS3D { 

    const int WaterFrameBuffers::REFLECTION_WIDTH = 320;
    const int WaterFrameBuffers::REFLECTION_HEIGHT = 180;
    const int WaterFrameBuffers::REFRACTION_WIDTH = 1280;
    const int WaterFrameBuffers::REFRACTION_HEIGHT = 720;
    NSEngine::FrameBuffer* WaterFrameBuffers::reflectionFrameBuffer;
    NSEngine::FrameBuffer* WaterFrameBuffers::refractionFrameBuffer;

    void WaterFrameBuffers::Init()
    {
        reflectionFrameBuffer = new NSEngine::FrameBuffer(REFLECTION_WIDTH, REFLECTION_HEIGHT, NSEngine::FrameBuffer::DEPTH_RENDER_BUFFER);
        refractionFrameBuffer = new NSEngine::FrameBuffer(REFLECTION_WIDTH, REFLECTION_HEIGHT, NSEngine::FrameBuffer::DEPTH_TEXTURE);
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
        glViewport(0, 0, NSEngine::engineData::gameWidth, NSEngine::engineData::gameHeight);
    }

}
