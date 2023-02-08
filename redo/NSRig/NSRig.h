#ifndef NSRIG_H_
#define NSRIG_H_

#include "NSRigBone.h"
#include "NSRigAnim.h"
#include <map>
#include <string>
#include <stack>

namespace NSEngine {

    class NSRig {

        public:
            NSRig();
            ~NSRig();

            void Update();
            void Draw();

            NSRigBone* getBone(std::string const& s);
            void addBone(std::string const& parent, std::string const& name, glm::vec3 const& pos1, glm::vec3 const& pos2, glm::mat4 const& mat);

            void setTransform(glm::mat4 const& mat);

            void pushMatrix(glm::mat4 const& mat) { matrices.push(mat); }
            void popMatrix() { if (!matrices.empty()) matrices.pop(); }
            glm::mat4 topMatrix() { if (matrices.empty()) return glm::mat4(1.f); return matrices.top(); }

            glm::mat4 getAnim(std::string const& bonename) { if (anim == nullptr) return glm::mat4(1.f); return anim->getInterpolatedMatrix(bonename, time, bones[bonename]->lastKeyFrame); }

            void setAnim(NSRigAnim* a) { anim = a; }

        private:
            NSRigBone* root;
            NSRigAnim* anim = nullptr;
            float time = 0.f;
            std::map<std::string const, NSRigBone*> bones;

            std::stack<glm::mat4> matrices;

            friend class NSRigBone;
    };

}

#endif // NSRIG_H_
