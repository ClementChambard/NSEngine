#include "HorizontalBlur.h"
#include "NSEngine.h"

namespace NSEngine {

    HorizontalBlur::HorizontalBlur() : shader(new HorizontalBlurShader()), renderer(new ImageRenderer()) 
    {
        shader->start();
        shader->SetTargetWidth(engineData::gameWidth);
        shader->stop();
    }
    HorizontalBlur::HorizontalBlur(int targetFboWidth, int targetFboHeight) : shader(new HorizontalBlurShader()), renderer(new ImageRenderer(targetFboWidth, targetFboHeight)) 
    {
        shader->start();
        shader->SetTargetWidth(targetFboWidth);
        shader->stop();
    }

    void HorizontalBlur::render(GLuint texture)
    {
        shader->start();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        renderer->renderQuad();
        shader->stop();
    }

}
