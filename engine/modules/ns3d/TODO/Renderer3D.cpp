#include "Renderer3D.h"
#include "../NSEngine.hpp"
#include "WaterFrameBuffers.h"

namespace NS3D {

    Renderer3D::Renderer3D()
    {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        objRenderer = new ObjectRenderer(new StaticShader());
        terRenderer = new TerrainRenderer(new TerrainShader());
        skyRenderer = new SkyboxRenderer(new SkyboxShader());
        watRenderer = new WaterRenderer(new WaterShader());
        WaterFrameBuffers::Init();
    }

    Renderer3D::~Renderer3D()
    {
        delete objRenderer;
        delete terRenderer;
        delete skyRenderer;
        delete watRenderer;
        WaterFrameBuffers::cleanUp();
    }

    void Renderer3D::prepare()
    {
        glEnable(GL_DEPTH_TEST);
        glClearColor(skyCol.r,skyCol.g,skyCol.b,1.f);   
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Renderer3D::setSkyColor(glm::vec3 color)
    {
        objRenderer->shader->start();
        objRenderer->shader->SetSkyColor(color);   
        objRenderer->shader->stop();
        terRenderer->shader->start();
        terRenderer->shader->SetSkyColor(color);   
        terRenderer->shader->stop();
        skyCol = color;
    }

    void Renderer3D::processObject(const Object3D* object, bool outlined)
    {
        TexturedModel* m = object->getModel();
        if (objectsToRender.find(m) == objectsToRender.end())
            objectsToRender.insert({m,{object}});
        else
            objectsToRender.at(m).push_back(object);

        if (outlined) outlinedObject = object;
    }

    void Renderer3D::processTerrain(const Terrain* terrain)
    {
        terrainsToRender.push_back(terrain);
    }

    void Renderer3D::processWaterTile(const WaterTile* water)
    {
        waterToRender.push_back(water);
    }

    void Renderer3D::processLight(const Light* l)
    {
        lights.push_back(l);
    }

    void Renderer3D::render(ns::FrameBuffer* fbo)
    {
        if (!waterToRender.empty())
        {
            glEnable(GL_CLIP_DISTANCE0);
            terRenderer->shader->start();
            terRenderer->shader->SetClipPlane({0,1,0,-waterToRender[0]->getHeight()-0.8f});
            terRenderer->shader->stop();
            objRenderer->shader->start();
            objRenderer->shader->SetClipPlane({0,1,0,-waterToRender[0]->getHeight()-0.8f});
            objRenderer->shader->stop();
            ns::activeCamera3D()->InvertY(waterToRender[0]->getHeight());
            WaterFrameBuffers::bindReflectionFrameBuffer();
            prepare();
            objRenderer->render(objectsToRender, lights);
            terRenderer->render(terrainsToRender, lights);
            skyRenderer->render();
            WaterFrameBuffers::unbindCurrentFrameBuffer();
            ns::activeCamera3D()->InvertY(waterToRender[0]->getHeight());
            terRenderer->shader->start();
            terRenderer->shader->SetClipPlane({0,-1,0,waterToRender[0]->getHeight()+0.8f});
            terRenderer->shader->stop();
            objRenderer->shader->start();
            objRenderer->shader->SetClipPlane({0,-1,0,waterToRender[0]->getHeight()+0.8f});
            objRenderer->shader->stop();
            WaterFrameBuffers::bindRefractionFrameBuffer();
            prepare();
            objRenderer->render(objectsToRender, lights);
            terRenderer->render(terrainsToRender, lights);
            skyRenderer->render();
            WaterFrameBuffers::unbindCurrentFrameBuffer();
            glDisable(GL_CLIP_DISTANCE0);
            terRenderer->shader->start();
            terRenderer->shader->SetClipPlane({0,0,0,0});
            terRenderer->shader->stop();
            objRenderer->shader->start();
            objRenderer->shader->SetClipPlane({0,0,0,0});
            objRenderer->shader->stop();
        }
        if (fbo != nullptr) fbo->bind();
        prepare();
        objRenderer->render(objectsToRender, lights);
        if (outlinedObject) objRenderer->renderOutlined(outlinedObject);
        terRenderer->render(terrainsToRender, lights);
        skyRenderer->render();
        watRenderer->render(waterToRender,lights.size()>0?lights[0]:nullptr);
        if (fbo != nullptr) fbo->unbind();
        waterToRender.clear();
        terrainsToRender.clear();
        objectsToRender.clear();
        lights.clear();
        outlinedObject = nullptr;
    }

}
