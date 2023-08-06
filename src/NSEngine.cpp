#include "NSEngine.h"
#include <cmath>
#include <glm/fwd.hpp>
#include "Engine.hpp"

namespace NSEngine {

    Camera2D* engineData::cam2d = nullptr;
    Camera3D* engineData::cam3d = nullptr;
    std::vector<SpriteBatch> engineData::layers;
    int* engineData::layerDrawOrder = nullptr;
    size_t engineData::targetLayer = 0;
    int engineData::debugLayer;
    std::vector<IEventProcessor*> engineData::eventProcessors;

    void toggleCulling(bool use)
    {
        if (use)
        {
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
        }
        else glDisable(GL_CULL_FACE);
    }

    void createCamera(int mode, int width, int height)
    {
        if (mode == 0) engineData::cam2d = new Camera2D(width, height);
        if (mode == 1) engineData::cam3d = new Camera3D(width,height);
        info("created new camera of type", mode);
    }

    Camera2D* activeCamera()
    {
        if (engineData::cam2d != nullptr)
            return engineData::cam2d;
        else return nullptr;
    }

    Camera3D* activeCamera3D()
    {
        if (engineData::cam3d != nullptr) return engineData::cam3d;
        else return nullptr;
    }

    glm::mat4 getCameraUniform(bool isstatic)
    {
        if (engineData::cam3d != nullptr)
        {
            return isstatic?engineData::cam3d->getCamStatic():engineData::cam3d->getCam();
        }
        else if (engineData::cam2d != nullptr)
        {
            return isstatic?engineData::cam2d->getCamStatic():engineData::cam2d->getCam();
        }
        return glm::mat4(1);
    }

    glm::mat4 getViewMatrix()
    {
        if (engineData::cam3d != nullptr)
        {
            return engineData::cam3d->getView();
        }
        return glm::mat4(1);
    }
    glm::vec3 camPos()
    {
        if (engineData::cam3d != nullptr)
        {
            return engineData::cam3d->getPosition();
        }
        return glm::vec3(0,0,0);
    }

    void setCamBoundaries(int w, int h)
    {
        auto data = getInstance()->window().getWindowData();
        if (engineData::cam3d != nullptr) engineData::cam3d->setWH(w,h,(float)w/(float)data.bwidth);
    }

    int addGameLayer(bool depthTest, bool is_static)
    {
        int id = engineData::layers.size();
        engineData::layers.emplace_back();
        engineData::layers.back().is_static = is_static;
        engineData::layers.back().depthTest = depthTest;
        info("added new graphics layer", id, "of type", std::to_string(is_static) + std::to_string(depthTest));
        return id;
    }

    void draw_set_layer(size_t layerID)
    {
        if (layerID >= engineData::layers.size())
        {
            error("no such graphics layer");
            return;
        }
        engineData::targetLayer = layerID;
    }

    void moveCameraTo(glm::vec3 position)
    {
        if (activeCamera() != nullptr) activeCamera()->setPosition(position);
        else if (activeCamera3D() != nullptr) activeCamera3D()->setPosition(position.x,position.y,position.z);
        else warning("no camera available");
    }

    SpriteBatch* getLayer(size_t i)
    {
        if (i >= engineData::layers.size())
        {
            error("no such graphics layer");
            return nullptr;
        }
        return &engineData::layers[i];
    }

    SpriteBatch* getTargetLayer()
    {
        return &engineData::layers[engineData::targetLayer];
    }
}
