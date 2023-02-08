#ifndef NSRIGEDITOR_H_
#define NSRIGEDITOR_H_

#include "NSRig.h"

namespace NSEngine {

    class NSRigEditor {

        public:
            NSRigEditor();
            ~NSRigEditor();

            void Update();
            void Draw();

        private:
            NSRig* current_rig;

            glm::vec3 pos = {5, 5, 5};
            glm::vec3 lookAt = glm::normalize(glm::vec3(-1, -1, -1));
            glm::vec3 focusPoint = {0, 0, 0};
            float pitch = 0.f;
            float yaw = 0.f;
            float zoom = 1.f;

    };

}

#endif // NSRIGEDITOR_H_
