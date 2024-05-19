#ifndef MODELDATA_INCLUDED_H
#define MODELDATA_INCLUDED_H

#include "../defines.h"

namespace NS3D {

class ModelData {
    public:
        ModelData(f32* v, f32* t, f32* n, u32* i, f32 fp, u32 nv, u32 ni) :
            vertices(v), textureCoords(t), normals(n),
            indices(i), furthestPoint(fp), numVertices(nv), numIndices(ni) {}
        ~ModelData() { delete vertices; delete normals; delete textureCoords; delete indices; }

        f32* getVertices() const { return vertices; }
        f32* getTextureCoords() const { return textureCoords; }
        f32* getNormals() const { return normals; }
        u32* getIndices() const { return indices; }
        f32 getFurthestPoint() const { return furthestPoint; }
        u32 getNumVertices() const { return numVertices; }
        u32 getNumIndices() const { return numIndices; }

    private:
        f32* vertices;
        f32* textureCoords;
        f32* normals;
        u32* indices;
        f32 furthestPoint;
        u32 numVertices;
        u32 numIndices;
};

}

#endif
