#include "Camera3D.h"
#include "InputManager.h"
#include <SDL2/SDL.h>
#include "Interpolator.h"

#define tempNormalize3 glm::vec3 temp = glm::normalize(glm::vec3(x, y, z));

namespace NSEngine {

    Camera3D::Camera3D(unsigned int screenW, unsigned int screenH)
    {
        scr_w = screenW;
        scr_h = screenH;
        pos = glm::vec3(0.f, 0.f, 0.9f * (float)scr_w / 2.f);
        lookat = glm::vec3(0.f, 0.f, -1.f);
        up = glm::vec3(0.f, 1.f, 0.f);
        fov = PI1_4;
        fogMin = 1000000.f;
        fogMax = 1000000.f;
        fogCol = Color(0, 0, 0, 0);
        orthoMatrix = glm::ortho(0.f, (float)scr_w, 0.f, (float)scr_h);
        persp = glm::perspective(fov, (float)scr_w / (float)scr_h, 0.1f, 10000.f);
        viewMatrix = glm::lookAt(pos, pos + lookat, up);
        cameraStaticMatrix = persp * viewMatrix;
        glm::vec3 right_vec = glm::cross(lookat, up);
        glm::vec3 up2 = glm::normalize(glm::cross(right_vec, lookat));
        viewMatrix = glm::lookAt(pos, pos + lookat, up2);
        perspS = glm::perspective(PI1_4, (float)scr_w / (float)scr_h, 0.1f, 10000.f);
        viewMatrixS = glm::lookAt(glm::vec3(0.f, 0.f, 0.9f * (float)scr_w + 2.f), glm::vec3(0.f, 0.f, 0.9f * (float)scr_w + 1.f), glm::vec3(0.f, 1.f, 0.f));
        cameraMatrix = persp * viewMatrix;
        cameraStaticMatrix = perspS * viewMatrixS;
    }

    Camera3D::~Camera3D() {}

    const bool debugprint = false;

    void Camera3D::setClearColor(float r, float g, float b)
    {
        clearcol = glm::vec3(r / 255.f, g / 255.f, b / 255.f);
        if(debugprint)std::cout << "setClearColor(" << r << ", " << g << ", " << b << ");\n";
    }

    void Camera3D::setPosition(float x, float y, float z)
    {
        pos = glm::vec3(x, y, z);
        if(debugprint)std::cout << "setPos(" << x << ", " << y << ", " << z << ");\n";
    }

static bool donotupdatenextframe = false;
    void Camera3D::setMat(glm::mat4 const& proj, glm::mat4 const& view) {
        persp = proj;
        viewMatrix = view;
        cameraMatrix = persp * view;
        donotupdatenextframe = true;
    }

    void Camera3D::PositionTime(uint16_t time, uint8_t mode, float x, float y, float z)
    {
        ITP_ (&pos, pos, glm::vec3(x, y, z), mode, time, false);
        if(debugprint)std::cout << "posTime(" << time << ", " << (int)mode << ", " << x << ", " << y << ", " << z << ");\n";
    }

    void Camera3D::PositionBezier(uint16_t time, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3)
    {
        InterpolateManager::InterpolateBezier(&pos, pos, {x2,y2,z2}, {x1,y1,z1}, {x3,y3,z3}, time);
        if(debugprint)std::cout << "posBez\n";
    }

    void Camera3D::setLook(float x, float y, float z)
    {
        lookat = glm::normalize(glm::vec3(x, y, z));
        if(debugprint)std::cout << "setLook(" << x << ", " << y << ", " << z << ");\n";
    }

    void Camera3D::LookTime(uint16_t time, uint8_t mode, float x, float y, float z)
    {
        ITP_ (&lookat, lookat, glm::normalize(glm::vec3(x, y, z)), mode, time, false);
        if(debugprint)std::cout << "lookTime(" << time << ", " << (int)mode << ", " << x << ", " << y << ", " << z << ");\n";
    }

    void Camera3D::LookBezier(uint16_t time, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3)
    {
        InterpolateManager::InterpolateBezier(&lookat, lookat, {x2,y2,z2}, {x1,y1,z1}, {x3,y3,z3}, time);
        if(debugprint)std::cout << "lookBez\n";
    }

    void Camera3D::setUp(float x, float y, float z)
    {
        up = glm::normalize(glm::vec3(x, y, z));
        if(debugprint)std::cout << "setUp(" << x << ", " << y << ", " << z << ");\n";
    }

    void Camera3D::setFov(float f)
    {
        fov = f;
        if(debugprint)std::cout << "setFov(" << f << ");\n";
    }

    void Camera3D::FovTime(uint16_t time, uint8_t mode, float f)
    {
        ITP_ (&fov, fov, f, mode, time, false);
        if(debugprint)std::cout << "fovTime(" << time << ", " << (int)mode << ", " << f << ");\n";
    }

    void Camera3D::setFog(uint8_t r, uint8_t g, uint8_t b, uint8_t a, float min, float max)
    {
        fogCol = Color(r, g, b, a);
        fogMin = min;
        fogMax = max;
        if(debugprint)std::cout << "setFog(" << (int)r << ", " << (int)g << ", " << (int)b << ", " << (int)a << ", " << min << ", " << max << ");\n";
    }

    void Camera3D::FogTime(uint16_t time, uint8_t mode, uint8_t r, uint8_t g, uint8_t b, uint8_t a, float min, float max)
    {
        ITP_ (&fogCol, fogCol, Color(r, g, b, a), mode, time, false);
        ITP_ (&fogMax, fogMax, max, mode, time, false);
        ITP_ (&fogMin, fogMin, min, mode, time, false);
        if(debugprint)std::cout << "fogTime(" << time << ", " << (int)mode << ", " << (int)r << ", " << (int)g << ", " << (int)b << ", " << (int)a << ", " << min << ", " << max << ");\n";
    }

    void Camera3D::UpTime(uint16_t time, uint8_t mode, float x, float y, float z)
    {
        ITP_ (&up, up, glm::normalize(glm::vec3(x, y, z)), mode, time, false);
        if(debugprint)std::cout << "upTime(" << time << ", " << (int)mode << ", " << x << ", " << y << ", " << z << ");\n";
    }

    void Camera3D::setWH(unsigned int w, unsigned int h, float factor)
    {
        scr_h = (unsigned int)((float)h / factor); scr_w = (unsigned int)((float)w / factor);
        calculate();
        perspS = glm::perspective(PI1_4, (float)scr_w / (float)scr_h, 0.1f, 10000.f);
        viewMatrixS = glm::lookAt(glm::vec3(0.f, 0.f, 0.9f * (float)scr_w + 2.f), glm::vec3(0.f, 0.f, 0.9f * (float)scr_w + 1.f), glm::vec3(0.f, 1.f, 0.f));
        cameraStaticMatrix = perspS * viewMatrixS;
    }

    void Camera3D::InvertY(float yy)
    {
        pitch *= -1;
        lookat = glm::vec3(cos(pitch) * sin(yaw), sin(pitch), cos(pitch) * cos(yaw));
        glm::vec3 right_vec = glm::cross(lookat, up);
        glm::vec3 up2 = glm::normalize(glm::cross(right_vec, lookat));
        pos.y = pos.y - 2 * (pos.y - yy);
        viewMatrix = glm::lookAt(pos, pos + lookat, up2);
    }

    void Camera3D::setDebugSpeed(float mov, float mous)
    {
        moveSpeed = mov;
        mouseSpeed = mous;
    }

    void Camera3D::Update(bool debugControl)
    {
    if (donotupdatenextframe) {
        donotupdatenextframe = false;
        return;
    }
        static bool isFirstFrame = true;
        if (debugControl)
        {
            static float dbgx = pos.x, dbgy = pos.y, dbgz = pos.z;
            if (!isFirstFrame) {
                static glm::vec3 debugPos = pos, debugLookat = lookat;
                yaw -= mouseSpeed * (Inputs::Mouse().guiPos.x + Inputs::Keyboard().Axis(NSK_q, NSK_e));
                pitch += mouseSpeed * (Inputs::Mouse().guiPos.y + Inputs::Keyboard().Axis(NSK_f, NSK_r));
                pitch = glm::clamp(pitch, -PI / 2.f + 0.001f, PI / 2.f - 0.001f);
                float keyHoriz = moveSpeed * Inputs::Keyboard().Axis(NSK_d, NSK_a);
                float keyVerti = moveSpeed * Inputs::Keyboard().Axis(NSK_s, NSK_w);
                debugLookat = glm::vec3(cos(pitch) * sin(yaw), sin(pitch), cos(pitch) * cos(yaw));
                glm::vec3 debugUp = glm::vec3(0,1,0);
                glm::vec3 debugRight = glm::normalize(glm::cross(debugLookat, debugUp));
                //glm::vec3 debugForward = glm::normalize(glm::cross(debugUp, debugRight));
                dbgx += keyHoriz * cos(yaw) + keyVerti * sin(yaw);
                dbgz += -keyHoriz * sin(yaw) + keyVerti * cos(yaw);
                dbgy += moveSpeed * Inputs::Keyboard().Axis(NSK_lshift, NSK_space);
                fov += 0.01f * Inputs::Keyboard().Axis(NSK_z, NSK_x);
                fov = glm::clamp(fov, 0.f, PI / 2.f);
                debugPos = glm::vec3(dbgx, dbgy, dbgz);

                persp = glm::perspective((float)fov, (float)scr_w / (float)scr_h, 0.1f, 10000.f);
                glm::vec3 up2 = glm::normalize(glm::cross(debugRight, debugLookat));
                viewMatrix = glm::lookAt(debugPos, debugPos + debugLookat, up2);
                cameraMatrix = persp * viewMatrix;
            }
            else Inputs::Mouse().Hide();
            Inputs::Mouse().SetPos(0, 0);
            isFirstFrame = false;
        }
        else if (!isFirstFrame)
        {
            isFirstFrame = true;
            Inputs::Mouse().Show();
        }
        else
        {
            pitch = asin(lookat.y);
            calculate();
        }

    }

    float Camera3D::getFov() const
    {
        return fov;
    }

    glm::vec3 Camera3D::getPosition() const
    {
        return pos;
    }

    glm::vec3 Camera3D::getLook() const
    {
        return glm::normalize(lookat);
    }

    glm::vec3 Camera3D::getUp() const
    {
        return up;
    }

    glm::vec3 Camera3D::getClearColor() const
    {
        return glm::vec4((float)fogCol.r, (float)fogCol.g, (float)fogCol.b, (float)fogCol.a) / 255.f;
        return clearcol;
    }

    glm::vec4 Camera3D::getFog(float& min, float& max) const
    {
        min = fogMin;
        max = fogMax;
        return glm::vec4((float)fogCol.r, (float)fogCol.g, (float)fogCol.b, (float)fogCol.a) / 255.f;
    }

    glm::mat4 Camera3D::getCam() const
    {
        return cameraMatrix;
    }

    glm::mat4 Camera3D::getView(bool is_static) const
    {
        return is_static ? viewMatrixS : viewMatrix;
    }

    glm::mat4 Camera3D::getProjection(bool is_static) const
    {
        return is_static ? perspS : persp;
    }

    glm::mat4 Camera3D::getCamStatic() const
    {
        return cameraStaticMatrix;
    }

    glm::mat4 Camera3D::getCamGui() const
    {
        return glm::translate(orthoMatrix, glm::vec3((float)scr_w / 2.f, (float)scr_h / 2.f, 0));
    }

    void Camera3D::calculate()
    {
        persp = glm::perspective((float)fov, (float)scr_w / (float)scr_h, 0.1f, 10000.f);
        glm::vec3 right_vec = glm::cross(lookat, up);
        glm::vec3 up2 = glm::normalize(glm::cross(right_vec, lookat));
        viewMatrix = glm::lookAt(pos, pos + lookat, up2);
        cameraMatrix = persp * viewMatrix;
    }

    glm::vec3 Camera3D::getCameraSpace(glm::vec3 const& pos) const
    {
        glm::vec4 pos4 = viewMatrix * glm::vec4(pos, 1.f);
        return glm::vec3(pos4) / pos4.w;
    }

    glm::vec3 Camera3D::getScreenSpace(glm::vec3 const& pos) const
    {
        glm::vec4 pos4 = cameraMatrix * glm::vec4(pos, 1.f);
        return glm::vec3(pos4) / pos4.w;
    }

}
