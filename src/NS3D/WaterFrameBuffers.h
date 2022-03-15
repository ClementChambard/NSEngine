#ifndef WATERFRAMEBUFFERS_INCLUDED_H
#define WATERFRAMEBUFFERS_INCLUDED_H

#include "../FrameBuffer.h"

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
            static const int REFLECTION_WIDTH;
            static const int REFRACTION_WIDTH;

        private:
            static const int REFLECTION_HEIGHT;
            static const int REFRACTION_HEIGHT;

            static NSEngine::FrameBuffer* reflectionFrameBuffer;
            static NSEngine::FrameBuffer* refractionFrameBuffer;

    };

}

#endif
