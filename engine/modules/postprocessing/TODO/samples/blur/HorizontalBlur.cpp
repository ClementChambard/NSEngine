#include "HorizontalBlur.h"
#include <NSEngine.hpp>

#include <GL/glew.h>

namespace ns {

    HorizontalBlur::HorizontalBlur() : shader(new HorizontalBlurShader()), renderer(new ImageRenderer()) 
    {
        shader->use();
        shader->set_target_width(get_main_window()->get_width());
    }
    HorizontalBlur::HorizontalBlur(u32 target_fbo_width, u32 target_fbo_height) : shader(new HorizontalBlurShader()), renderer(new ImageRenderer(target_fbo_width, target_fbo_height))
    {
        shader->use();
        shader->set_target_width(target_fbo_width);
    }

    void HorizontalBlur::render(GLuint texture)
    {
        shader->use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        renderer->render_quad();
    }

}
