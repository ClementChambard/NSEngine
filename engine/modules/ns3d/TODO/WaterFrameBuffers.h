#ifndef WATERFRAMEBUFFERS_INCLUDED_H
#define WATERFRAMEBUFFERS_INCLUDED_H

#include "../FrameBuffer.h"
#include "../defines.h"

namespace NS3D { 

class WaterFrameBuffers {
    public:
        static void Init();
        static void cleanUp();

        static void bindReflectionFrameBuffer();
        static void bindRefractionFrameBuffer();
        static void unbindCurrentFrameBuffer();

        static GLuint getReflectionTexture() { return reflectionFrameBuffer->getColorTexture(); }//{ return reflectionTexture; }
        static GLuint getRefractionTexture() { return refractionFrameBuffer->getColorTexture(); }//{ return refractionTexture; }
        static GLuint getRefractionDepthTexture() { return refractionFrameBuffer->getDepthTexture(); }//{ return refractionDepthTexture; }

    protected:
        static constexpr i32 REFLECTION_WIDTH = 320;
        static constexpr i32 REFRACTION_WIDTH = 1280;

    private:
        static const i32 REFLECTION_HEIGHT = 180;
        static const i32 REFRACTION_HEIGHT = 720;

        static ns::FrameBuffer* reflectionFrameBuffer;
        static ns::FrameBuffer* refractionFrameBuffer;

};

}

#endif
