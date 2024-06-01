#include "VerticalBlur.h"
#include "NSEngine.h"

namespace NSEngine {

    VerticalBlur::VerticalBlur() : shader(new VerticalBlurShader()), renderer(new ImageRenderer()) 
    {
        shader->start();
        shader->SetTargetWidth(engineData::gameHeight);
        shader->stop();
    }
    VerticalBlur::VerticalBlur(int targetFboWidth, int targetFboHeight) :shader(new VerticalBlurShader()), renderer(new ImageRenderer(targetFboWidth, targetFboHeight)) 
    {
        shader->start();
        shader->SetTargetWidth(targetFboHeight);
        shader->stop();
    }

    void VerticalBlur::render(GLuint texture)
    {
        shader->start();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        renderer->renderQuad();
        shader->stop();
    }

}
