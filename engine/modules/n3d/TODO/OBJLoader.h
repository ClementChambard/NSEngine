#ifndef OBJLOADER_HEADER_INCLUDED
#define OBJLOADER_HEADER_INCLUDED

#include "RawModel.h"
#include <defines.h>

namespace ns::n3d::OBJLoader {
RawModel* load_obj_model(cstr file_name);
}

#endif // OBJLOADER_HEADER_INCLUDED
