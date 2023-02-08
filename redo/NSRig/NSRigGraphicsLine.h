#ifndef NSRIGGRAPHICSLINE_H_
#define NSRIGGRAPHICSLINE_H_

#include "NSRigGraphics.h"

namespace NSEngine {

    class NSRigGraphicsLine : public NSRigGraphics {

        public:
            NSRigGraphicsLine(glm::vec3 const& p1, glm::vec3 const& p2, float s = 1);
            ~NSRigGraphicsLine();

            void Draw(glm::mat4 const& mat) override;

        private:
            glm::vec3 pos1;
            glm::vec3 pos2;
            float size;
    };

}


#endif // NSRIGGRAPHICSLINE_H_
