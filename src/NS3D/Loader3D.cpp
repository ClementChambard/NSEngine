#include "Loader3D.h"
#include "../ImageLoader.h"

namespace NS3D {

    std::vector<GLuint> Loader3D::createdVaos;
    std::vector<GLuint> Loader3D::createdVbos;
    std::vector<GLuint> Loader3D::createdTexs;

    RawModel* Loader3D::loadToVAO(f32*        positions,     u32 positionsL,
                                  f32*        textureCoords, u32 textureCoordsL,
                                  f32*        normals,       u32 normalsL,
                                  u32* indices,       u32 indicesL)
    {
        i32 vaoID = createVAO();
        bindIndicesBuffer(indices, indicesL);
        storeDataInAttributeList(0, 3, positions, positionsL);
        storeDataInAttributeList(1, 2, textureCoords, textureCoordsL);
        storeDataInAttributeList(2, 3, normals, normalsL);
        unbindVAO();
        delete[] positions;
        delete[] textureCoords;
        delete[] normals;
        delete[] indices;
        return new RawModel(vaoID, indicesL);
    }

    RawModel* Loader3D::loadToVAO(f32* positions, u32 positionsL, u32 dimensions)
    {
        i32 vaoID = createVAO();
        storeDataInAttributeList(0, dimensions, positions, positionsL);
        unbindVAO();
        return new RawModel(vaoID, positionsL / dimensions);
    }

    RawModel* Loader3D::loadToVAO(ModelData* md)
    {
        i32 vaoID = createVAO();
        u32 nI = md->getNumIndices();
        u32 nV = md->getNumVertices();
        bindIndicesBuffer(md->getIndices(), nI);
        storeDataInAttributeList(0, 3, md->getVertices(), nV*3);
        storeDataInAttributeList(1, 2, md->getTextureCoords(), nV*2);
        storeDataInAttributeList(2, 3, md->getNormals(), nV*3);
        unbindVAO();
        delete md;
        return new RawModel(vaoID, nI);
    }


    GLuint Loader3D::loadCubeMap(const std::vector<cstr> images)
    {
        GLuint texID;
        glGenTextures(1, &texID);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, texID);
        for (u32 i = 0; i < images.size(); i++)
        {
            i32 w, h, n;
            bytes buf = ns::ImageLoader::imageToBuffer(images[i], w, h, n);
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, w, h, 0,
                    GL_RGBA, GL_UNSIGNED_BYTE, buf);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        createdTexs.push_back(texID);
        return texID;
    }

    GLuint Loader3D::createVAO()
    {
        GLuint vaoID;
        glGenVertexArrays(1, &vaoID);
        createdVaos.push_back(vaoID);
        glBindVertexArray(vaoID);
        return vaoID;
    }

    void Loader3D::cleanUp()
    {
        for (GLuint vao : createdVaos)
            glDeleteVertexArrays(1, &vao);

        for (GLuint vbo : createdVbos)
            glDeleteBuffers(1, &vbo);

        for (GLuint tex : createdTexs)
            glDeleteTextures(1, &tex);

    }

    void Loader3D::storeDataInAttributeList(i32 attributeNumber, u32 dimensions, f32* data, u32 dataL)
    {
        GLuint vboID;
        glGenBuffers(1, &vboID);
        createdVbos.push_back(vboID);
        glBindBuffer(GL_ARRAY_BUFFER, vboID);
        glBufferData(GL_ARRAY_BUFFER, dataL*sizeof(f32), data, GL_STATIC_DRAW);
        glVertexAttribPointer(attributeNumber, dimensions, GL_FLOAT, GL_FALSE, 0, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void Loader3D::unbindVAO()
    {
        glBindVertexArray(0);
    }

    void Loader3D::bindIndicesBuffer(u32* indices, u32 indicesL)
    {
        GLuint vboID;
        glGenBuffers(1, &vboID);
        createdVbos.push_back(vboID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesL*sizeof(i32), indices, GL_STATIC_DRAW);
    }

}



