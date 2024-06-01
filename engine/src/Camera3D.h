#ifndef CAMERA3D_INCLUDED_H
#define CAMERA3D_INCLUDED_H

#include <glm/glm.hpp>
#include "color.h"
#include "./defines.h"

#define NS_CAM_3D 1

namespace ns {

class Camera3D {
    public:
        Camera3D(u32 screenW, u32 screenH);
        ~Camera3D();

        void setClearColor(f32 r, f32 g, f32 b);
        void setPosition(f32 x, f32 y, f32 z);
        void setLook(f32 x, f32 y, f32 z);
        void setUp(f32 x, f32 y, f32 z);
        void setFov(f32 f);
        void setFog(u8 r, u8 g, u8 b, u8 a, f32 min, f32 max);
        void setMat(glm::mat4 const& proj, glm::mat4 const& view);

        void setWH(u32 w, u32 h, f32 factor = 1);
        void InvertY(f32 yy);
        void setDebugSpeed(f32 mov, f32 mous);

        void Update(bool debugControl = false);

        void setMatStatic(glm::mat4 const& m) {cameraStaticMatrix = m;}

        f32 getFov() const;
        glm::vec3 getPosition() const;
        glm::vec3 getLook() const;
        glm::vec3 getUp() const;
        glm::vec3 getClearColor() const;
        glm::vec4 getFog(f32& min, f32& max) const;
        glm::mat4 getCam() const;
        glm::mat4 getView(bool is_static = false) const;
        glm::mat4 getProjection(bool is_static = false) const;
        glm::mat4 getCamStatic() const;
        glm::mat4 getCamGui() const;

        glm::vec3 getCameraSpace(glm::vec3 const& pos) const;
        glm::vec3 getScreenSpace(glm::vec3 const& pos) const;

    private:
        f32 moveSpeed = 2.f, mouseSpeed = 0.01f;
        u32 scr_w = 0, scr_h = 0;
        f32 fov;
        glm::vec3 pos;
        glm::vec3 lookat;
        glm::vec3 up;
        Color fogCol;
        f32 fogMin;
        f32 fogMax;
        f32 pitch = 0.f;
        f32 yaw = 0.f;
        glm::vec3 clearcol = glm::vec3(0,0,0);
        glm::mat4 orthoMatrix;
        glm::mat4 viewMatrix;
        glm::mat4 viewMatrixS;
        glm::mat4 cameraMatrix;
        glm::mat4 cameraStaticMatrix;
        glm::mat4 persp;
        glm::mat4 perspS;
        void calculate();

};

}
#endif
