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
            static uint32_t gameflags;
            static FpsLimiter* fps;
            static Camera2D* cam2d;
            static Camera3D* cam3d;
            static SDL_Event event;
            static SDL_Window* window;
            static SDL_GLContext context;
            static Window* NSWindow;
            static std::vector<SpriteBatch> layers;
            static size_t targetLayer;
            static int gameWidth;
            static int gameHeight;
            static float displayRatio;
            static int displaymode;
            static int debugLayer;
            static std::vector<IEventProcessor*> eventProcessors;
    };

   /**
    * Initialise the engine
    */
    extern void Init();

   /**
    * Initialise the subsystems of the engine
    */
    extern void Subsystems();

   /**
    * Ends the initialisation of the engine
    */
    extern void EndInit();

   /**
    * Quits the engine
    */
    extern void Quit();

   /**
    * Returns if the engine is running
    */
    extern bool IsRunning();

   /**
    * Stops the engine from running
    */
    extern void StopRunning();

   /**
    * Changes the maximum amount of frame per seconds
    */
    extern void setMaxFps(int fps);

   /**
    * Toggles the debug informations
    */
    extern void toggleDebugInfo();

   /**
    * Toggles wireframe rendering
    */
    extern void toggleWireframe();

   /**
    * Toggles frame by frame mode
    */
    extern void toggleFBF();

   /**
    * Toggles free camera controls
    */
    extern void toggle3DCameraControl();

   /**
    * Toggles culling (back face ?)
    */
    extern void toggleCulling(bool use);

   /**
    * Returns if the game is in frame by frame mode
    */
    extern bool IsFBF();

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
    * Starts the current game frame
    */
    extern void StartFrame();

   /**
    * Ends the current game frame
    */
    extern void EndFrame();

   /**
    * Starts the update process of the engine
    */
    extern void StartUpdate();

   /**
    * Updates everything related to the engine
    */
    extern void UpdateEngine(float framespeed = 1);

   /**
    * Ends the update process of the engine
    */
    extern void EndUpdate();

   /**
    * Renders everything related to the engine
    */
    extern void RenderEngine();

   /**
    * Moves the current camera to position
    */
    extern void moveCameraTo(glm::vec3 position);

}

#endif
