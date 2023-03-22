#include "OBJLoader.h"
#include "Loader3D.h"
#include "../Error.h"
#include "../FileOpener.h"
#include "OBJFileLoader.h"

namespace NS3D {

    RawModel* OBJLoader::loadObjModel(std::string fileName)
    {

        if (!NSEngine::FileOpener::openFileLines(fileName))
        {
            NSEngine::error("Couldn't open OBJ file : " + fileName);
        }

        std::vector<glm::vec3> vertexPositions;
        std::vector<glm::vec3> vertexNormals;
        std::vector<glm::vec2> vertexTexcoords;
        std::vector<unsigned int> indices;
        float* positions = nullptr;
        float* normals = nullptr;
        float* texcoords = nullptr;

        std::string line;
        while(NSEngine::FileOpener::readLine(line))
        {
            std::vector<std::string> currentLine = stringSplit(line);
            if (stringBeginWith(line, "v "))
            {
                glm::vec3 vertexpos = glm::vec3(std::stof(currentLine[1]), std::stof(currentLine[2]), std::stof(currentLine[3]));
                vertexPositions.push_back(vertexpos);
            }
            else if (stringBeginWith(line, "vt "))
            {
                glm::vec2 vertextex = glm::vec2(std::stof(currentLine[1]), std::stof(currentLine[2]));
                vertexTexcoords.push_back(vertextex);
            }
            else if (stringBeginWith(line, "vn "))
            {
                glm::vec3 vertexnor = glm::vec3(std::stof(currentLine[1]), std::stof(currentLine[2]), std::stof(currentLine[3]));
                vertexNormals.push_back(vertexnor);
            }
            else if (stringBeginWith(line, "f "))
            {
                if (positions == nullptr)
                {
                    positions = new float[3*vertexPositions.size()];
                    normals = new float[3*vertexPositions.size()];
                    texcoords = new float[2*vertexPositions.size()];
                }
                std::vector<std::string> v1 = stringSplit(currentLine[1], '/');
                std::vector<std::string> v2 = stringSplit(currentLine[2], '/');
                std::vector<std::string> v3 = stringSplit(currentLine[3], '/');
                processVertex(v1, indices, vertexTexcoords, vertexNormals, texcoords, normals);
                processVertex(v2, indices, vertexTexcoords, vertexNormals, texcoords, normals);
                processVertex(v3, indices, vertexTexcoords, vertexNormals, texcoords, normals);
            }
        }

        for (size_t i = 0; i < vertexPositions.size(); i++)
        {
            positions[i*3    ] = vertexPositions[i].x;
            positions[i*3 + 1] = vertexPositions[i].y;
            positions[i*3 + 2] = vertexPositions[i].z;
        }

        unsigned int* indicesAr = new unsigned int[indices.size()];
        for (size_t i = 0; i < indices.size(); i++) indicesAr[i] = indices[i];

        return Loader3D::loadToVAO(positions, vertexPositions.size()*3,
                                   texcoords, vertexPositions.size()*2,
                                   normals,   vertexPositions.size()*3,
                                   indicesAr, indices.size());
    }

    void OBJLoader::processVertex(std::vector<std::string> vertex, std::vector<unsigned int>& indices, std::vector<glm::vec2>& texcoords, 
            std::vector<glm::vec3>& normals, float* texcoordsAr, float* normalsAr)
    {
        int currentVertexPointer = std::stoi(vertex[0]) - 1;
        indices.push_back(currentVertexPointer);

        glm::vec2 currentTex = texcoords[std::stoi(vertex[1]) - 1];
        texcoordsAr[currentVertexPointer*2    ] = currentTex.x;
        texcoordsAr[currentVertexPointer*2 + 1] = currentTex.y;
     
        glm::vec3 currentNor = normals[std::stoi(vertex[2]) - 1];
        normalsAr[currentVertexPointer*3    ] = currentNor.x;
        normalsAr[currentVertexPointer*3 + 1] = currentNor.y;
        normalsAr[currentVertexPointer*3 + 2] = currentNor.z;
    } 

}
