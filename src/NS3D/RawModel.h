#ifndef RAWMODEL_INCLUDED_H
#define RAWMODEL_INCLUDED_H

#include "../defines.h"

namespace NS3D {

class RawModel {
    public:
        RawModel(i32 vaoid, i32 vertexcount) :
            vaoID(vaoid), vertexCount(vertexcount) {}

        i32 getVaoID() const { return vaoID; }
        i32 getVertexCount() const { return vertexCount; }

    private:
        i32 vaoID;
        i32 vertexCount;
};

}

#endif
