#ifndef PPEFFECT_INCLUDED_H
#define PPEFFECT_INCLUDED_H

#include "FrameBuffer.h"
#include "ShaderProgram.h"

namespace NSEngine {

    class PPEffect {
        public:
            virtual void render(GLuint texture) {}
            virtual GLuint getOutputTexture() { return 0; }

            virtual void cleanup() {}

            class ImageRenderer {
                public:
                    ImageRenderer(unsigned int w, unsigned int h) : fbo(new FrameBuffer(w, h, FrameBuffer::NONE)) {}
                    ImageRenderer() {}
                    ~ImageRenderer() { if (fbo != nullptr) delete fbo; }

                    void renderQuad()
                    {
                        if (fbo != nullptr) fbo->bind();
                        glClear(GL_COLOR_BUFFER_BIT);
                        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
                        if (fbo != nullptr) fbo->unbind();
                    }

                    GLuint getOutputTexture() const { if (fbo != nullptr) return fbo->getColorTexture(); return 0; }


                private:
                    FrameBuffer* fbo = nullptr;

            };

    };

}

#endif
