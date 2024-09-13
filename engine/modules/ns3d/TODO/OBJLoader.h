#ifndef OBJLOADER_INCLUDED_H
#define OBJLOADER_INCLUDED_H

#include "RawModel.h"
#include "../defines.h"

namespace NS3D {

    class OBJLoader {
        public:
            static RawModel* loadObjModel(cstr fileName);

    };

}

#endif
