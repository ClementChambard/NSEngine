#ifndef RAWMODEL_INCLUDED_H
#define RAWMODEL_INCLUDED_H

namespace NS3D {

    class RawModel {
        public:
            RawModel(int vaoid, int vertexcount) :
                vaoID(vaoid), vertexCount(vertexcount) {}

            int getVaoID() const { return vaoID; }
            int getVertexCount() const { return vertexCount; }

        private:
            int vaoID;
            int vertexCount;
    };

}

#endif
