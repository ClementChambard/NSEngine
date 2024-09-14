#include "Renderer3D.h"
#include "WaterFrameBuffers.h"
#include <GL/glew.h>

namespace ns::n3d {

Renderer3D::Renderer3D()
{
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    obj_renderer = new ObjectRenderer(new StaticShader());
    ter_renderer = new TerrainRenderer(new TerrainShader());
    sky_renderer = new SkyboxRenderer(new SkyboxShader());
    wat_renderer = new WaterRenderer(new WaterShader());
    WaterFrameBuffers::init();
}

Renderer3D::~Renderer3D()
{
    delete obj_renderer;
    delete ter_renderer;
    delete sky_renderer;
    delete wat_renderer;
    WaterFrameBuffers::cleanup();
}

void Renderer3D::prepare()
{
    glEnable(GL_DEPTH_TEST);
    glClearColor(sky_col.r,sky_col.g,sky_col.b,1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer3D::set_sky_color(vec3 color)
{
    obj_renderer->shader->use();
    obj_renderer->shader->set_sky_color(color);
    ter_renderer->shader->use();
    ter_renderer->shader->set_sky_color(color);
    sky_col = color;
}

void Renderer3D::process_object(const Object3D* object, bool outlined)
{
    TexturedModel* m = object->get_model();
    if (objects_to_render.find(m) == objects_to_render.end())
        objects_to_render.insert({m,{object}});
    else
        objects_to_render.at(m).push_back(object);

    if (outlined) outlined_object = object;
}

void Renderer3D::process_terrain(const Terrain* terrain)
{
    terrains_to_render.push_back(terrain);
}

void Renderer3D::process_water_tile(const WaterTile* water)
{
    water_to_render.push_back(water);
}

void Renderer3D::process_light(const Light* l)
{
    lights.push_back(l);
}

void Renderer3D::render(ns::FrameBuffer* fbo)
{
    if (!water_to_render.empty())
    {
        glEnable(GL_CLIP_DISTANCE0);
        ter_renderer->shader->use();
        ter_renderer->shader->set_clip_plane({0,1,0,-water_to_render[0]->get_height()-0.8f});
        obj_renderer->shader->use();
        obj_renderer->shader->set_clip_plane({0,1,0,-water_to_render[0]->get_height()-0.8f});
        // ns::activeCamera3D()->InvertY(water_to_render[0]->get_height());
        WaterFrameBuffers::bind_reflection_frame_buffer();
        prepare();
        obj_renderer->render(objects_to_render, lights);
        ter_renderer->render(terrains_to_render, lights);
        sky_renderer->render();
        WaterFrameBuffers::unbind_current_frame_buffer();
        // ns::activeCamera3D()->InvertY(waterToRender[0]->getHeight());
        ter_renderer->shader->use();
        ter_renderer->shader->set_clip_plane({0,-1,0,water_to_render[0]->get_height()+0.8f});
        obj_renderer->shader->use();
        obj_renderer->shader->set_clip_plane({0,-1,0,water_to_render[0]->get_height()+0.8f});
        WaterFrameBuffers::bind_refraction_frame_buffer();
        prepare();
        obj_renderer->render(objects_to_render, lights);
        ter_renderer->render(terrains_to_render, lights);
        sky_renderer->render();
        WaterFrameBuffers::unbind_current_frame_buffer();
        glDisable(GL_CLIP_DISTANCE0);
        ter_renderer->shader->use();
        ter_renderer->shader->set_clip_plane({0,0,0,0});
        obj_renderer->shader->use();
        obj_renderer->shader->set_clip_plane({0,0,0,0});
    }
    if (fbo != nullptr) fbo->bind();
    prepare();
    obj_renderer->render(objects_to_render, lights);
    if (outlined_object) obj_renderer->render_outlined(outlined_object);
    ter_renderer->render(terrains_to_render, lights);
    sky_renderer->render();
    wat_renderer->render(water_to_render,lights.size()>0?lights[0]:nullptr);
    if (fbo != nullptr) fbo->unbind();
    water_to_render.clear();
    terrains_to_render.clear();
    objects_to_render.clear();
    lights.clear();
    outlined_object = nullptr;
}

}
