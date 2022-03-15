#ifndef MODELDATA_INCLUDED_H
#define MODELDATA_INCLUDED_H

namespace NS3D {

    class ModelData {
        public:
            ModelData(float* v, float* t, float* n, unsigned int* i, float fp, unsigned int nv, unsigned int ni) :
                vertices(v), textureCoords(t), normals(n),
                indices(i), furthestPoint(fp), numVertices(nv), numIndices(ni) {}
            ~ModelData() { delete vertices; delete normals; delete textureCoords; delete indices; }

            float* getVertices() const { return vertices; }
            float* getTextureCoords() const { return textureCoords; }
            float* getNormals() const { return normals; }
            unsigned int* getIndices() const { return indices; }
            float getFurthestPoint() const { return furthestPoint; }
            unsigned int getNumVertices() const { return numVertices; }
            unsigned int getNumIndices() const { return numIndices; }

        private:
            float* vertices;
            float* textureCoords;
            float* normals;
            unsigned int* indices;
            float furthestPoint;
            unsigned int numVertices;
            unsigned int numIndices;
    };

}

#endif
