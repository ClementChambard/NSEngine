#ifndef LOADER3D_INCLUDED_H
#define LOADER3D_INCLUDED_H

#include "RawModel.h"
#include "ModelData.h"
#include <GL/glew.h>
#include <vector>
#include <string>

namespace NS3D {

    class Loader3D {
        public:
            static RawModel* loadToVAO(float*        positions,      unsigned int positionsL, 
                                       float*        textureCoords,  unsigned int textureCoordsL,
                                       float*        normals,        unsigned int normalsL,
                                       unsigned int* indices,        unsigned int indicesL);
            static RawModel* loadToVAO(float*        positions,      unsigned int positionsL,
                                       unsigned int  dimensions);
            static RawModel* loadToVAO(ModelData*    md);

            static GLuint createVAO();

            //Right Left Top Bottom Back Front
            static GLuint loadCubeMap(const std::vector<std::string> images);

            static void cleanUp();

        private:
            static void storeDataInAttributeList(int attributeNumber, unsigned int dimensions, float* data, unsigned int dataL);
            static void unbindVAO();
            static void bindIndicesBuffer(unsigned int* indices, unsigned int indicesL);

            static std::vector<GLuint> createdVaos;
            static std::vector<GLuint> createdVbos;
            static std::vector<GLuint> createdTexs;
    };

}

#endif
