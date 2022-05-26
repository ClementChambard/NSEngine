#ifndef NSRIGBONE_H_
#define NSRIGBONE_H_

#include "NSRigAnim.h"
#include "NSRigGraphics.h"

#include <glm/glm.hpp>
#include <vector>
#include <string>

namespace NSEngine {

    class NSRig;

    class NSRigBone {

        public:
            NSRigBone(NSRig* rig, std::string name, NSRigBone* parent, glm::vec3 const& pos1, glm::vec3 const& pos2, glm::mat4 const& mat);
            NSRigBone(NSRig* rig, std::string name, NSRigBone* parent, glm::mat4 const& mat);
            NSRigBone(NSRig* rig);
            ~NSRigBone();

            void Update();
            void Draw();

            void addChild(NSRigBone* b);
            void setAnimate(glm::mat4 const& mat) { animate = mat; }

            void addGraphics(NSRigGraphics* g) { graphics.push_back(g); }

            void attachRig(NSRig* r) { attachedRig = r; }
            void unAttachRig() { attachedRig = nullptr; }

            glm::mat4 getSelf() const { return self; }

            glm::vec3 transformPoint(glm::vec3 const& v) { glm::vec4 a = self * glm::vec4(v, 1.f); return glm::vec3(a) / a.w; }

            NSRigAnim::KeyFrame* lastKeyFrame = nullptr;

        private:
            std::string name = "";
            NSRigBone* parent = nullptr;
            NSRig* rig = nullptr;
            NSRig* attachedRig = nullptr;

            glm::mat4 animate = glm::mat4(1.f);
            glm::mat4 propagate = glm::mat4(1.f);
            glm::mat4 self = glm::mat4(1.f);

            std::vector<NSRigBone*> childs;
            std::vector<NSRigGraphics*> graphics;

            friend class NSRig;
            friend class NSRigAnim;
    };

}

#endif // NSRIGBONE_H_
