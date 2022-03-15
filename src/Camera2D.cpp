#include "Camera2D.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <iostream>
namespace NSEngine {

    float CameraSpeed = 8.f;

    Camera2D::Camera2D(int screenW, int screenH) 
    {
        orthoMatrix = glm::ortho(0.f, (float) screenW, 0.f, (float) screenH);
        scr_h = screenH;
        scr_w = screenW;
        position = {0,0,0};
        positionto = {0,0,0};
    }
 
    Camera2D::~Camera2D() {}

    void Camera2D::setPosition(const glm::vec3& pos) 
    {
        positionto = pos; 
    }

    void Camera2D::setScale(const float& scal) 
    { 
        scaleto = scal; 
    }

    glm::vec3 Camera2D::getPosition() 
    { 
        return position; 
    }

    glm::vec3 Camera2D::getPositionTo() 
    {
        return positionto; 
    }

    float Camera2D::getScale() 
    { 
        return scale; 
    }

    glm::vec2 Camera2D::getSize() 
    { 
        return {scr_w*scale,scr_h*scale};
    }

    void Camera2D::Update()
    {
        if (scaleto != scale || positionto != position)
        {
            scale += (scaleto - scale)/CameraSpeed;
            glm::vec3 diff = {positionto.x-position.x,positionto.y-position.y,0};
            if (CameraSpeed*CameraSpeed > ((diff.x*diff.x)+(diff.y*diff.y))) position = positionto;
            else position += CameraSpeed*glm::normalize(diff);
            cameraMatrix = glm::translate(orthoMatrix, glm::vec3(-position.x + scr_w / 2.f, -position.y + scr_h / 2.f, 0.f));
            glm::mat4 scaleMat = glm::scale(glm::mat4(1.f), glm::vec3(scale, scale, scale));
            cameraMatrix = scaleMat * cameraMatrix;
        }
    }

    glm::mat4 Camera2D::getCam() 
    { 
        return cameraMatrix; 
    }

    glm::mat4 Camera2D::getCamStatic() 
    { 
        return glm::translate(orthoMatrix, glm::vec3(scr_w/2, scr_h/2, 0)); 
    }

    glm::mat4 Camera2D::getCamGUI() 
    { 
        return getCamStatic(); 
    }

    glm::vec4 Camera2D::getBounds() 
    { 
        return glm::vec4(
                glm::vec2(position.x,position.y)-glm::vec2(scale * scr_w/2,scale * scr_h/2),
                glm::vec2(position.x,position.y)+glm::vec2(scale * scr_w/2,scale * scr_h/2)
            ); 
    }
}
