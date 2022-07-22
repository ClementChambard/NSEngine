#ifndef CAMERA3D_INCLUDED_H
#define CAMERA3D_INCLUDED_H

#include <glm/glm.hpp>
#include <glm/fwd.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "vertex.h"
#include "math/math.h"

#define NS_CAM_3D 1

namespace NSEngine {

    class Camera3D {
        public:
            Camera3D(unsigned int screenW, unsigned int screenH);
            ~Camera3D();

            void setClearColor(float r, float g, float b);
            void setPosition(float x, float y, float z);                             //2
            void PositionTime(uint16_t time, uint8_t mode, float x, float y, float z);        //3
            void PositionBezier(uint16_t time, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);
            void setLook(float x, float y, float z);                                    //4
            void LookTime(uint16_t time, uint8_t mode, float x, float y, float z);               //5
            void LookBezier(uint16_t time, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);
            void setUp(float x, float y, float z);                                      //6
            void setFov(float f);                                                       //7
            void FovTime(uint16_t time, uint8_t mode, float f);
            void setFog(uint8_t r, uint8_t g, uint8_t b, uint8_t a, float min, float max);              //8
            void FogTime(uint16_t time, uint8_t mode, uint8_t r, uint8_t g, uint8_t b, uint8_t a, float min, float max); //9
            void UpTime(uint16_t time, uint8_t mode, float x, float y, float z);                 //18

            void setWH(unsigned int w, unsigned int h, float factor = 1);
            void InvertY(float yy);
            void setDebugSpeed(float mov, float mous);

            void Update(bool debugControl = false);

            float getFov() const;
            glm::vec3 getPosition() const;
            glm::vec3 getLook() const;
            glm::vec3 getUp() const;
            glm::vec3 getClearColor() const;
            glm::vec4 getFog(float& min, float& max) const;
            glm::mat4 getCam() const;
            glm::mat4 getView(bool is_static = false, bool is_gui = false) const;
            glm::mat4 getProjection(bool is_static = false, bool is_gui = false) const;
            glm::mat4 getCamStatic() const;
            glm::mat4 getCamGui() const;

            glm::vec3 getCameraSpace(glm::vec3 const& pos) const;
            glm::vec3 getScreenSpace(glm::vec3 const& pos) const;

        private:
            float moveSpeed = 2.f, mouseSpeed = 0.01f;
            unsigned int scr_w = 0, scr_h = 0;
            float fov;
            glm::vec3 pos;
            glm::vec3 lookat;
            glm::vec3 up;
            Color fogCol;
            float fogMin;
            float fogMax;
            float pitch = 0.f;
            float yaw = 0.f;
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
