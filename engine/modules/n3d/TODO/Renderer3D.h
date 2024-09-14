#ifndef RENDERER3D_HEADER_INCLUDED
#define RENDERER3D_HEADER_INCLUDED

#include <FrameBuffer.h>
#include "ObjectRenderer.h"
#include "SkyboxRenderer.h"
#include "TerrainRenderer.h"
#include "WaterRenderer.h"

namespace ns::n3d {

class Renderer3D {
    public:
        Renderer3D();
        ~Renderer3D();

        void prepare();
        void render(ns::FrameBuffer* fbo = nullptr);

        void process_terrain(const Terrain* terrain);
        void process_object(const Object3D* object, bool outlined = false);
        void process_water_tile(const WaterTile* water);
        void process_light(const Light* l);

        void set_skybox(u32 tex_id) { sky_renderer->set_texture(tex_id); }
        void set_sky_color(vec3 color);

    private:
        ObjectRenderer* obj_renderer;
        TerrainRenderer* ter_renderer;
        SkyboxRenderer* sky_renderer;
        WaterRenderer* wat_renderer;

        vec3 sky_col = vec3(0,0,0);

        std::map<const TexturedModel*, std::vector<const Object3D*>> objects_to_render;
        const Object3D* outlined_object = nullptr;
        std::vector<const Terrain*> terrains_to_render;
        std::vector<const WaterTile*> water_to_render;
        std::vector<const Light*> lights;

};

} // namespace ns::n3d

#endif // RENDERER3D_HEADER_INCLUDED
