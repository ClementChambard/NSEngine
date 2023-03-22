#include "Shockwave.h"
#include "NSEngine.h"

namespace NSEngine {

    ShockwaveEffect::ShockwaveEffect() : ShockwaveEffect(engineData::gameWidth, engineData::gameHeight) {}

    ShockwaveEffect::ShockwaveEffect(int targetFboWidth, int targetFboHeight) : shader(new ShockwaveEffectShader()), wshader(new ShockwaveShader()),
                renderer(new ImageRenderer(targetFboWidth, targetFboHeight)), waveRenderer(new ImageRenderer(targetFboWidth, targetFboHeight))
    {
        glGenVertexArrays(1, &vaoID);
        glBindVertexArray(vaoID);
        glGenBuffers(1, &vboID);
        glBindBuffer(GL_ARRAY_BUFFER, vboID);
        glBufferData(GL_ARRAY_BUFFER, (2 + 2) * sizeof(float)*4, nullptr, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(2*4*sizeof(float)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void ShockwaveEffect::renderWaves()
    {
        // Init drawing on framebuffer
        glEnable(GL_BLEND);
        glBlendFunc(GL_DST_COLOR, GL_SRC_COLOR);
        waveRenderer->getFbo()->bind();
        glClearColor(0.5f,0.5f,1.f,1.f);
        glClear(GL_COLOR_BUFFER_BIT);
        wshader->start();
        glBindVertexArray(vaoID);
        glBindBuffer(GL_ARRAY_BUFFER, vboID);
        for (unsigned int i = 0; i < NB_OF_WAVES; i++)
        {
            if (!wavePool[i].active) continue;
            wavePool[i].Render();
        }
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        wshader->stop();
        waveRenderer->getFbo()->unbind();
        glBindTexture(GL_TEXTURE_2D, 0);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        // Reset drawing on framebuffer
    }

    void ShockwaveEffect::render(GLuint texture)
    {
        aberration = 0.0f;
        shader->start();
        shader->setAberration(aberration);
        shader->setStrength(strength);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, waveRenderer->getOutputTexture());
        shader->setTextures();
        renderer->renderQuad();
        shader->stop();
    }

    void ShockwaveEffect::AddWave(GLuint texID, const glm::vec4 &uvQuad, const glm::vec3 &pos, float speed, float attenuation)
    {
        int i = 0;
        while (i < NB_OF_WAVES && wavePool[(lastUsedWave+i)%NB_OF_WAVES].active) i++;
        wavePool[(lastUsedWave+i)%NB_OF_WAVES].init(texID, uvQuad, pos, speed, attenuation);
        wavePool[(lastUsedWave+i)%NB_OF_WAVES].parent = this;
        lastUsedWave = (lastUsedWave+i+1)%NB_OF_WAVES;
    }

    void ShockwaveEffect::Shockwave::init(GLuint texID, const glm::vec4 &uvQuad, const glm::vec3 &pos, float speed, float attenuation)
    {
        this->texID = texID;
        this->uvQuad = uvQuad;
        this->pos = pos;
        this->speed = speed;
        this->attenuation = attenuation;
        this->active = true;
        this->alpha = 1;
        this->size = 0;
    }

    void ShockwaveEffect::Shockwave::Render()
    {
        float sizeY = 16.f/9.f*size;
        glm::vec3 pos3 = NSEngine::activeCamera3D()->getScreenSpace(this->pos);
        if (pos3.z > 0 && pos3.z < 1) {
            glm::vec2 pos = pos3;
            glm::vec2 Pos_tl = pos + glm::vec2(-size/2, sizeY/2);
            glm::vec2 Pos_tr = pos + glm::vec2( size/2, sizeY/2);
            glm::vec2 Pos_br = pos + glm::vec2( size/2,-sizeY/2);
            glm::vec2 Pos_bl = pos + glm::vec2(-size/2,-sizeY/2);
            float data[] = { Pos_tr.x, Pos_tr.y,   Pos_tl.x, Pos_tl.y,
                            Pos_br.x, Pos_br.y,   Pos_bl.x, Pos_bl.y,
                            uvQuad.z, uvQuad.y,   uvQuad.x, uvQuad.y,
                            uvQuad.z, uvQuad.w,   uvQuad.x, uvQuad.w };

            glBufferSubData(GL_ARRAY_BUFFER, 0, 4*4*sizeof(float), data);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texID);
            parent->wshader->SetAttenuation(alpha);

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
