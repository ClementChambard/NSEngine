#ifndef LOADER3D_INCLUDED_H
#define LOADER3D_INCLUDED_H

#include "RawModel.h"
#include "ModelData.h"
#include <GL/glew.h>
#include <vector>
#include "../defines.h"

namespace NS3D {

    class Loader3D {
        public:
            static RawModel* loadToVAO(f32* positions,      u32 positionsL, 
                                       f32* textureCoords,  u32 textureCoordsL,
                                       f32* normals,        u32 normalsL,
                                       u32* indices,        u32 indicesL);
            static RawModel* loadToVAO(f32* positions,      u32 positionsL,
                                       u32  dimensions);
            static RawModel* loadToVAO(ModelData* md);

            static GLuint createVAO();

            //Right Left Top Bottom Back Front
            static GLuint loadCubeMap(const std::vector<cstr> images);

            static void cleanUp();

        private:
            static void storeDataInAttributeList(i32 attributeNumber, u32 dimensions, f32* data, u32 dataL);
            static void unbindVAO();
            static void bindIndicesBuffer(u32* indices, u32 indicesL);

            static std::vector<GLuint> createdVaos;
            static std::vector<GLuint> createdVbos;
            static std::vector<GLuint> createdTexs;
    };

}

#endif
