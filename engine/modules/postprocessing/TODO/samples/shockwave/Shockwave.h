#ifndef SHOCKWAVE_HEADER_INCLUDED
#define SHOCKWAVE_HEADER_INCLUDED

#include "../../PPEffect.h"
#include "./ShockwaveEffectShader.h"
#include "./ShockwaveShader.h"
#include <FrameBuffer.h>

#include <functional>

namespace ns {

    class ShockwaveEffect : public PPEffect {
        public:
            ShockwaveEffect();
            ShockwaveEffect(u32 target_fbo_width, u32 target_fbo_height);

            void cleanup() override { delete shader; delete renderer; }
            void render(u32 texture) override;
            void render_waves();
            u32 get_output_texture() override { return renderer->get_output_texture(); }

            void add_wave(u32 tex_id, vec4 const& uv_quad, vec3 const& pos, f32 speed, f32 attenuation);

            class Shockwave {

                public:
                    Shockwave() {}
                    ~Shockwave() {}

                    void init(u32 tex_id, vec4 const& uv_quad, vec3 const& pos, f32 speed, f32 attenuation);

                    void set_update(std::function<void(Shockwave*)> const& u) { update = u; }

                    void render();

                private:
                    u32 tex_id = 0;
                    vec4 uv_quad;
                    vec3 pos;
                    f32 speed = 0;
                    f32 attenuation = 0;
                    f32 size = 0;
                    f32 alpha = 1;
                    bool active = false;
                    ShockwaveEffect* parent;

                    std::function<void(Shockwave*)> update = nullptr;

                    friend class ShockwaveEffect;
            };

        private:
            static constexpr unsigned int NB_OF_WAVES = 20;
            ShockwaveEffectShader* shader;
            ShockwaveShader* wshader;
            ImageRenderer* renderer;
            ImageRenderer* wave_renderer;
            u32 last_used_wave;
            Shockwave* wave_pool = new Shockwave[NB_OF_WAVES];
            u32 vbo_id, vao_id;

            f32 aberration = 1;
            f32 strength = 1/30.f;

            friend class Shockwave;
    };

}


#endif // SHOCKWAVE_HEADER_INCLUDED
