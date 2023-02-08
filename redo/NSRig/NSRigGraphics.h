#ifndef NSRIGGRAPHICS_H_
#define NSRIGGRAPHICS_H_

#include <glm/glm.hpp>

namespace NSEngine
{

    class NSRigGraphics {

        public:
            NSRigGraphics() {}
            virtual ~NSRigGraphics() {}

            virtual void Draw(glm::mat4 const& mat) {}

    };

}

#endif // NSRIGGRAPHICS_H_
