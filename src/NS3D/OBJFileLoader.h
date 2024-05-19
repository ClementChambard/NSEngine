#ifndef OBJFILELOADER_INCLUDED_H
#define OBJFILELOADER_INCLUDED_H

#include "../defines.h"
#include "ModelData.h"

namespace NS3D {

class OBJFileLoader {
public:
  static ModelData *loadOBJ(cstr fileName);
};

} // namespace NS3D

#endif
