#include "Shockwave.h"
#include <NSEngine.hpp>

#include <GL/glew.h>

namespace ns {

    ShockwaveEffect::ShockwaveEffect() : ShockwaveEffect(get_main_window()->get_width(), get_main_window()->get_height()) {}

    ShockwaveEffect::ShockwaveEffect(u32 target_fbo_width, u32 target_fbo_height) : shader(new ShockwaveEffectShader()), wshader(new ShockwaveShader()),
                renderer(new ImageRenderer(target_fbo_width, target_fbo_height)), wave_renderer(new ImageRenderer(target_fbo_width, target_fbo_height))
    {
        glGenVertexArrays(1, &vao_id);
        glBindVertexArray(vao_id);
        glGenBuffers(1, &vbo_id);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
        glBufferData(GL_ARRAY_BUFFER, (2 + 2) * sizeof(float)*4, nullptr, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(2*4*sizeof(float)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void ShockwaveEffect::render_waves()
    {
        // Init drawing on framebuffer
        glEnable(GL_BLEND);
        glBlendFunc(GL_DST_COLOR, GL_SRC_COLOR);
        wave_renderer->get_fbo()->bind();
        glClearColor(0.5f,0.5f,1.f,1.f);
        glClear(GL_COLOR_BUFFER_BIT);
        wshader->use();
        glBindVertexArray(vao_id);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
        for (unsigned int i = 0; i < NB_OF_WAVES; i++)
        {
            if (!wave_pool[i].active) continue;
            wave_pool[i].render();
        }
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        wave_renderer->get_fbo()->unbind();
        glBindTexture(GL_TEXTURE_2D, 0);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        // Reset drawing on framebuffer
    }

    void ShockwaveEffect::render(u32 texture)
    {
        aberration = 0.0f;
        shader->use();
        shader->set_aberration(aberration);
        shader->set_strength(strength);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, wave_renderer->get_output_texture());
        shader->set_textures();
        renderer->render_quad();
    }

    void ShockwaveEffect::add_wave(u32 tex_id, const vec4 &uv_quad, const vec3 &pos, float speed, float attenuation)
    {
        u32 i = 0;
        while (i < NB_OF_WAVES && wave_pool[(last_used_wave+i)%NB_OF_WAVES].active) i++;
        wave_pool[(last_used_wave+i)%NB_OF_WAVES].init(tex_id, uv_quad, pos, speed, attenuation);
        wave_pool[(last_used_wave+i)%NB_OF_WAVES].parent = this;
        last_used_wave = (last_used_wave+i+1)%NB_OF_WAVES;
    }

    void ShockwaveEffect::Shockwave::init(u32 tex_id, const vec4 &uv_quad, const vec3 &pos, float speed, float attenuation)
    {
        this->tex_id = tex_id;
        this->uv_quad = uv_quad;
        this->pos = pos;
        this->speed = speed;
        this->attenuation = attenuation;
        this->active = true;
        this->alpha = 1;
        this->size = 0;
    }

    void ShockwaveEffect::Shockwave::render()
    {
        float size_y = 16.f/9.f*size;
        vec3 pos3 = vec3(); // NSEngine::activeCamera3D()->getScreenSpace(this->pos);
        if (pos3.z > 0 && pos3.z < 1) {
            vec2 pos = vec2(pos3);
            vec2 pos_tl = pos + vec2(-size/2, size_y/2);
            vec2 pos_tr = pos + vec2( size/2, size_y/2);
            vec2 pos_br = pos + vec2( size/2,-size_y/2);
            vec2 pos_bl = pos + vec2(-size/2,-size_y/2);
            float data[] = { pos_tr.x, pos_tr.y,   pos_tl.x, pos_tl.y,
                             pos_br.x, pos_br.y,   pos_bl.x, pos_bl.y,
                            uv_quad.z, uv_quad.y,   uv_quad.x, uv_quad.y,
                            uv_quad.z, uv_quad.w,   uv_quad.x, uv_quad.w };

            glBufferSubData(GL_ARRAY_BUFFER, 0, 4*4*sizeof(float), data);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, tex_id);
            parent->wshader->set_attenuation(alpha);

            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        }
        if (update) update(this);
        else {
            alpha -= attenuation;
            size += speed;
            if (alpha <= 0) active = false;
        }
    }

}
