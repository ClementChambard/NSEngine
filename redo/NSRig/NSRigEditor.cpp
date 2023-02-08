#include "NSRigEditor.h"

#include "../NSEngine.h"
#include "../InputManager.h"
#include "../math/math.h"

namespace NSEngine {

    NSRigEditor::NSRigEditor()
    {
        current_rig = new NSRig();
        pos = glm::vec3(1, 1, 1);
        lookAt = glm::normalize(glm::vec3(-1, -1, -1));
        focusPoint = glm::vec3(0, 0, 0);
        pitch = 0.f;
        yaw = 0.f;
        zoom = 1.f;

        current_rig->addBone("root", "body", {0,0,0}, {0,20,0}, glm::translate(glm::mat4(1.f), glm::vec3(0,20,0)));
        current_rig->addBone("body", "head", {0,0,0}, {0,5,0}, glm::translate(glm::mat4(1.f), glm::vec3(0,5,0)));
        NSRigAnim* anim = new NSRigAnim();
        anim->setDuration(120);
        anim->addKeyFrame(NSRigAnim::KeyFrame("body",  0, {-10,0,0}, {0,0,0}, {1,1,1}, 9));
        anim->addKeyFrame(NSRigAnim::KeyFrame("body", 30, {0,0,10}, {0,0,0}, {1,1,1}, 9));
        anim->addKeyFrame(NSRigAnim::KeyFrame("body", 60, {10,0,0}, {0,0,0}, {2,2,2}, 9));
        anim->addKeyFrame(NSRigAnim::KeyFrame("body", 90, {0,0,-10}, {0,0,0}, {2,2,2}, 9));
        current_rig->setAnim(anim);
    }

    NSRigEditor::~NSRigEditor()
    {
        delete current_rig;
    }

    float t = 0.f;

    void NSRigEditor::Update()
    {

        if (Inputs::Mouse().Down(NSM_middleclick))
        {
            if (Inputs::Keyboard().Down(NSK_lshift))
            {
                float horAn = yaw - PI1_2;
                glm::vec3 vecHoriz = {sin(horAn), 0.f, cos(horAn)};
                glm::vec3 vecVerti = glm::normalize(glm::cross(vecHoriz, lookAt));
                focusPoint += zoom * -(vecHoriz*Inputs::Mouse().posDiff.x + vecVerti*Inputs::Mouse().posDiff.y);
            }
            else
            {
                yaw   -= 0.03 * Inputs::Mouse().posDiff.x;
                pitch += 0.03 * Inputs::Mouse().posDiff.y;

                if      (yaw   >=   PI2         ) yaw  -=  PI2;
                else if (yaw   <      0         ) yaw  +=  PI2;
                if      (pitch >  PI1_2 - 0.001f) pitch =  PI1_2 - 0.001f;
                else if (pitch < -PI1_2 + 0.001f) pitch = -PI1_2 + 0.001f;
            }
        }

        zoom += .1f * (Inputs::Mouse().scrollDown - Inputs::Mouse().scrollUp);

        pos = focusPoint - zoom * 900 * glm::vec3(sin(yaw)*cos(pitch), sin(pitch), cos(yaw)*cos(pitch));
        lookAt = focusPoint - pos;

        NSEngine::activeCamera3D()->setPosition(   pos.x,    pos.y,    pos.z);
        NSEngine::activeCamera3D()->setLook    (lookAt.x, lookAt.y, lookAt.z);

        current_rig->getBone("body")->setAnimate(glm::rotate(glm::mat4(1.f), t, glm::vec3(1,0,0)));
        t+=0.02f;
        current_rig->Update();
    }

    void NSRigEditor::Draw()
    {
        draw_set_layer(0);
        current_rig->Draw();
        glm::vec3 offset = {-.4f*zoom, -.4f*zoom, -.4f*zoom};
        float length = zoom * 25.f;
        NSEngine::draw_AA_box(focusPoint - offset, focusPoint + offset + glm::vec3(length, 0, 0), {255,   0,   0, 255}, false, false);
        NSEngine::draw_AA_box(focusPoint - offset, focusPoint + offset + glm::vec3(0, length, 0), {  0, 255,   0, 255}, false, false);
        NSEngine::draw_AA_box(focusPoint - offset, focusPoint + offset + glm::vec3(0, 0, length), {  0,   0, 255, 255}, false, false);
    }
}
