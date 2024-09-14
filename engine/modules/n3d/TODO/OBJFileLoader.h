#ifndef OBJFILELOADER_HEADER_INCLUDED
#define OBJFILELOADER_HEADER_INCLUDED

#include <defines.h>
#include "ModelData.h"

namespace ns::n3d::OBJFileLoader {

ModelData *load_obj(cstr file_name);

} // namespace ns::n3d::OBjFileLoader

#endif // OBJFILELOADER_HEADER_INCLUDED
