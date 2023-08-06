#include <SDL2/SDL.h>

#define NS_USE_ALL

#ifdef NS_USE_ALL
#define NS_USE_INTERPOLATOR
#endif

#include "InputManager.h"
#include "TextureManager.h"
#include "Window.h"
#include "DrawFuncs.h"
#include "Camera2D.h"
#include "Camera3D.h"
#include "Timing.h"

#ifdef NS_USE_INTERPOLATOR
#include "Interpolator.h"
#endif

#ifndef NSENGINE_INCLUDED_H
#define NSENGINE_INCLUDED_H

enum assets {NS_TEXTURE = 0, NS_SPRITE, NS_FONT, NS_SPRITEFONT, NS_TILESET, NS_SFX, NS_MUSIC};

namespace NSEngine {

   /**
    * This class contains the necessary variables of the engine
    */
    class engineData {
        public:
            static Camera2D* cam2d;
            static Camera3D* cam3d;
            static std::vector<SpriteBatch> layers;
            static int* layerDrawOrder;
            static size_t targetLayer;
            static int debugLayer;
            static std::vector<IEventProcessor*> eventProcessors;
    };

   /**
    * Toggles culling (back face ?)
    */
    extern void toggleCulling(bool use);

   /**
    * Creates a camera of mode mode and size width*height
    */
    extern void createCamera(int mode, int width, int height);

   /**
    * Gets the current active camera
    */
    extern Camera2D* activeCamera();

   /**
    * Gets the current active camera3d
    */
    extern Camera3D* activeCamera3D();

   /**
    * Gets the current camera's matrix
    */
    extern glm::mat4 getCameraUniform(bool isstatic);

   /**
    * Gets the current camera's view matrix
    */
    extern glm::mat4 getViewMatrix();

   /**
    * Gets the current camera's position
    */
    extern glm::vec3 camPos();

   /**
    * Sets the current camera's boundaries
    */
    extern void setCamBoundaries(int w, int h);

   /**
    * Adds a new graphics layer
    */
    extern int addGameLayer(bool depthTest = false, bool is_static = false);

   /**
    * Sets the graphics layer to draw to
    */
    extern void draw_set_layer(size_t layerID);

   /**
    * Moves the current camera to position
    */
    extern void moveCameraTo(glm::vec3 position);

   /**
    * Gets a layer
    */
    extern SpriteBatch* getLayer(size_t i);

   /**
    * Gets the target layer
    */
    extern SpriteBatch* getTargetLayer();
}

#endif
