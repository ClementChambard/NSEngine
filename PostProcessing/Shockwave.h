#ifndef SHOCKWAVE_H_
#define SHOCKWAVE_H_

#include "PPEffect.h"
#include "ShockwaveEffectShader.h"
#include "ShockwaveShader.h"
#include "FrameBuffer.h"

#include <functional>

namespace NSEngine {

    class ShockwaveEffect : public PPEffect {
        public:
            ShockwaveEffect();
            ShockwaveEffect(int targetFboWidth, int targetFboHeight);

            void cleanup() override { delete shader; delete renderer; }
            void render(GLuint texture) override;
            void renderWaves();
            GLuint getOutputTexture() override { return renderer->getOutputTexture(); }

            void AddWave(GLuint texID, glm::vec4 const& uvQuad, glm::vec3 const& pos, float speed, float attenuation);

            class Shockwave {

                public:
                    Shockwave() {}
                    ~Shockwave() {}

                    void init(GLuint texID, glm::vec4 const& uvQuad, glm::vec3 const& pos, float speed, float attenuation);

                    void setUpdate(std::function<void(Shockwave*)> const& u) { update = u; }

                    void Render();

                private:
                    GLuint texID = 0;
                    glm::vec4 uvQuad;
                    glm::vec3 pos;
                    float speed = 0;
                    float attenuation = 0;
                    float size = 0;
                    float alpha = 1;
                    bool active = false;
                    ShockwaveEffect* parent;

                    std::function<void(Shockwave*)> update = nullptr;

                    friend class ShockwaveEffect;
            };

        private:
            static constexpr unsigned int NB_OF_WAVES = 20;
            ImageRenderer* renderer;
            ImageRenderer* waveRenderer;
            ShockwaveEffectShader* shader;
            ShockwaveShader* wshader;
            unsigned int lastUsedWave;
            Shockwave* wavePool = new Shockwave[NB_OF_WAVES];
            GLuint vboID, vaoID;

            float aberration = 1;
            float strength = 1/30.f;

            friend class Shockwave;
    };

}


#endif // SHOCKWAVE_H_
