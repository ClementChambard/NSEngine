#include "Camera3D.h"
#include "InputManager.h"
#include <SDL2/SDL.h>
#include "math/math.h"
#include <glm/fwd.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define tempNormalize3 glm::vec3 temp = glm::normalize(glm::vec3(x, y, z));

namespace ns {

Camera3D::Camera3D(u32 screenW, u32 screenH)
{
    scr_w = screenW;
    scr_h = screenH;
    pos = glm::vec3(0.f, 0.f, 0.9f * (f32)scr_w / 2.f);
    lookat = glm::vec3(0.f, 0.f, -1.f);
    up = glm::vec3(0.f, 1.f, 0.f);
    fov = PI1_4;
    fogMin = 1000000.f;
    fogMax = 1000000.f;
    fogCol = Color(0, 0, 0, 0);
    orthoMatrix = glm::ortho(0.f, (f32)scr_w, 0.f, (f32)scr_h);
    persp = glm::perspective(fov, (f32)scr_w / (f32)scr_h, 0.1f, 10000.f);
    viewMatrix = glm::lookAt(pos, pos + lookat, up);
    cameraStaticMatrix = persp * viewMatrix;
    glm::vec3 right_vec = glm::cross(lookat, up);
    glm::vec3 up2 = glm::normalize(glm::cross(right_vec, lookat));
    viewMatrix = glm::lookAt(pos, pos + lookat, up2);
    perspS = glm::perspective(PI1_4, (f32)scr_w / (f32)scr_h, 0.1f, 10000.f);
    viewMatrixS = glm::lookAt(glm::vec3(0.f, 0.f, 0.9f * (f32)scr_w + 2.f), glm::vec3(0.f, 0.f, 0.9f * (f32)scr_w + 1.f), glm::vec3(0.f, 1.f, 0.f));
    cameraMatrix = persp * viewMatrix;
    cameraStaticMatrix = perspS * viewMatrixS;
}

Camera3D::~Camera3D() {}

void Camera3D::setClearColor(f32 r, f32 g, f32 b)
{
    clearcol = glm::vec3(r / 255.f, g / 255.f, b / 255.f);
}

void Camera3D::setPosition(f32 x, f32 y, f32 z)
{
    pos = glm::vec3(x, y, z);
}

static bool donotupdatenextframe = false;
void Camera3D::setMat(glm::mat4 const& proj, glm::mat4 const& view) {
    persp = proj;
    viewMatrix = view;
    cameraMatrix = persp * view;
    donotupdatenextframe = true;
}

void Camera3D::setLook(f32 x, f32 y, f32 z)
{
    lookat = glm::normalize(glm::vec3(x, y, z));
}

void Camera3D::setUp(f32 x, f32 y, f32 z)
{
    up = glm::normalize(glm::vec3(x, y, z));
}

void Camera3D::setFov(f32 f)
{
    fov = f;
}

void Camera3D::setFog(u8 r, u8 g, u8 b, u8 a, f32 min, f32 max)
{
    fogCol = Color(r, g, b, a);
    fogMin = min;
    fogMax = max;
}

void Camera3D::setWH(u32 w, u32 h, f32 factor)
{
    scr_h = (u32)((f32)h / factor); scr_w = (u32)((f32)w / factor);
    calculate();
    perspS = glm::perspective(PI1_4, (f32)scr_w / (f32)scr_h, 0.1f, 10000.f);
    viewMatrixS = glm::lookAt(glm::vec3(0.f, 0.f, 0.9f * (f32)scr_w + 2.f), glm::vec3(0.f, 0.f, 0.9f * (f32)scr_w + 1.f), glm::vec3(0.f, 1.f, 0.f));
    cameraStaticMatrix = perspS * viewMatrixS;
}

void Camera3D::InvertY(f32 yy)
{
    pitch *= -1;
    lookat = glm::vec3(cos(pitch) * sin(yaw), sin(pitch), cos(pitch) * cos(yaw));
    glm::vec3 right_vec = glm::cross(lookat, up);
    glm::vec3 up2 = glm::normalize(glm::cross(right_vec, lookat));
    pos.y = pos.y - 2 * (pos.y - yy);
    viewMatrix = glm::lookAt(pos, pos + lookat, up2);
}

void Camera3D::setDebugSpeed(f32 mov, f32 mous)
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
        static f32 dbgx = pos.x, dbgy = pos.y, dbgz = pos.z;
        if (!isFirstFrame) {
            static glm::vec3 debugPos = pos, debugLookat = lookat;
            yaw -= mouseSpeed * (Inputs::Mouse().guiPos.x + Inputs::Keyboard().Axis(NSK_q, NSK_e));
            pitch += mouseSpeed * (Inputs::Mouse().guiPos.y + Inputs::Keyboard().Axis(NSK_f, NSK_r));
            pitch = glm::clamp(pitch, -PI / 2.f + 0.001f, PI / 2.f - 0.001f);
            f32 keyHoriz = moveSpeed * Inputs::Keyboard().Axis(NSK_d, NSK_a);
            f32 keyVerti = moveSpeed * Inputs::Keyboard().Axis(NSK_s, NSK_w);
            debugLookat = glm::vec3(cos(pitch) * sin(yaw), sin(pitch), cos(pitch) * cos(yaw));
            glm::vec3 debugUp = glm::vec3(0,1,0);
            glm::vec3 debugRight = glm::normalize(glm::cross(debugLookat, debugUp));
            
            dbgx += keyHoriz * cos(yaw) + keyVerti * sin(yaw);
            dbgz += -keyHoriz * sin(yaw) + keyVerti * cos(yaw);
            dbgy += moveSpeed * Inputs::Keyboard().Axis(NSK_lshift, NSK_space);
            fov += 0.01f * Inputs::Keyboard().Axis(NSK_z, NSK_x);
            fov = glm::clamp(fov, 0.f, PI / 2.f);
            debugPos = glm::vec3(dbgx, dbgy, dbgz);

            persp = glm::perspective((f32)fov, (f32)scr_w / (f32)scr_h, 0.1f, 10000.f);
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

f32 Camera3D::getFov() const
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
    return glm::vec4((f32)fogCol.r, (f32)fogCol.g, (f32)fogCol.b, (f32)fogCol.a) / 255.f;
    return clearcol;
}

glm::vec4 Camera3D::getFog(f32& min, f32& max) const
{
    min = fogMin;
    max = fogMax;
    return glm::vec4((f32)fogCol.r, (f32)fogCol.g, (f32)fogCol.b, (f32)fogCol.a) / 255.f;
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
    return glm::translate(orthoMatrix, glm::vec3((f32)scr_w / 2.f, (f32)scr_h / 2.f, 0));
}

void Camera3D::calculate()
{
    persp = glm::perspective((f32)fov, (f32)scr_w / (f32)scr_h, 0.1f, 10000.f);
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
