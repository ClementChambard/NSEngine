#include "OBJFileLoader.h"

#include "../FileOpener.h"
#include "../Error.h"
#include "../StringUtil.h"

namespace NS3D {


    ModelData* OBJFileLoader::loadOBJ(std::string fileName)
    {
        if (!NSEngine::FileOpener::openFileLines(fileName))
        {
            NSEngine::error("Couldn't open OBJ file : " + fileName);
        }
        NSEngine::info("[NS3D] Starting reading file " + fileName);

        std::string line;
        std::vector<Vertex*> vertices;
        std::vector<glm::vec2> texcoords;
        std::vector<glm::vec3> normals;
        std::vector<unsigned int> indices;

        while(NSEngine::FileOpener::readLine(line))
        {
            std::vector<std::string> currentLine = stringSplit(line);
            if (stringBeginWith(line, "v "))
            {
                glm::vec3 vertexpos = glm::vec3(std::stof(currentLine[1]), std::stof(currentLine[2]), std::stof(currentLine[3]));
                vertices.push_back(new Vertex(vertices.size(), vertexpos));
            }
            else if (stringBeginWith(line, "vt "))
            {
                glm::vec2 vertextex = glm::vec2(std::stof(currentLine[1]), std::stof(currentLine[2]));
                texcoords.push_back(vertextex);
            }
            else if (stringBeginWith(line, "vn "))
            {
                glm::vec3 vertexnor = glm::vec3(std::stof(currentLine[1]), std::stof(currentLine[2]), std::stof(currentLine[3]));
                normals.push_back(vertexnor);
            }
            else if (stringBeginWith(line, "f "))
            {
                std::vector<std::string> v1 = stringSplit(currentLine[1],'/');
                std::vector<std::string> v2 = stringSplit(currentLine[2],'/');
                std::vector<std::string> v3 = stringSplit(currentLine[3],'/');
                processVertex(v1, vertices, indices);
                processVertex(v2, vertices, indices);
                processVertex(v3, vertices, indices);
            }
        }
        NSEngine::info("[NS3D] finished reading file.");
        removeUnusedVertices(vertices);
        float* verticesAr = new float[vertices.size() * 3];
        float* texturesAr = new float[vertices.size() * 2];
        float* normalsAr = new float[vertices.size() * 3];
        float furthestPoint = convertDataToArrays(vertices, texcoords, normals, verticesAr, texturesAr, normalsAr);
        unsigned int numId;
        unsigned int* indicesAr = convertIndicesToArray(indices, numId);
        ModelData* data = new ModelData(verticesAr, texturesAr, normalsAr, indicesAr, furthestPoint, vertices.size(), indices.size());
        NSEngine::info("[NS3D] model generated!");
        return data;
    }


    void OBJFileLoader::processVertex(std::vector<std::string> vertex, std::vector<Vertex*> vertices, std::vector<unsigned int> indices)
    {
        unsigned int id = std::stoi(vertex[0]) -1;
        Vertex* currentVertex = vertices[id];
        int tid = std::stoi(vertex[1]) -1;
        int nid = std::stoi(vertex[2]) -1;
        if (!currentVertex->isSet()) {
            currentVertex->setTextureIndex(tid);
            currentVertex->setNormalIndex(nid);
        } else {
            dealWithAlreadyProcessedVertex(currentVertex, tid, nid, indices, vertices);
        }
    }

    unsigned int* OBJFileLoader::convertIndicesToArray(std::vector<unsigned int> indices, unsigned int& nb)
    {
        unsigned int* ar = new unsigned int[indices.size()];
        for (unsigned int i = 0; i < indices.size(); i++)
            ar[i] = indices[i];
        nb = indices.size();
        return ar;
    }
    
    float OBJFileLoader::convertDataToArrays(std::vector<Vertex*> vertices, std::vector<glm::vec2> tex, std::vector<glm::vec3> nor, float* vertAr, float* texAr, float* norAr)
    {
        float furthestPoint = 0;
        for (int i = 0; i < vertices.size(); i++)
        {
            Vertex* currentVertex = vertices[i];
            if (currentVertex->getLength() > furthestPoint)
                furthestPoint = currentVertex->getLength();
            glm::vec3 pos = currentVertex->getPosition();
            glm::vec2 texcoords = tex[currentVertex->getTextureIndex()];
            glm::vec3 normal = nor[currentVertex->getNormalIndex()];
            vertAr[i * 3    ] = pos.x;
            vertAr[i * 3 + 1] = pos.y;
            vertAr[i * 3 + 2] = pos.z;
            texAr[i * 2    ] = texcoords.x;
            texAr[i * 2 + 1] = texcoords.y;
            norAr[i * 3    ] = normal.x;
            norAr[i * 3 + 1] = normal.y;
            norAr[i * 3 + 2] = normal.z;
        }
        return furthestPoint;
    }

    void OBJFileLoader::dealWithAlreadyProcessedVertex(Vertex* previousVertex, int newTexture, int newNormal, std::vector<unsigned int> indices, std::vector<Vertex*> vertices)
    {
        if (previousVertex->hasSameTextureAndNormal(newTexture, newNormal)) indices.push_back(previousVertex->getIndex());
        else
        {
            Vertex* anotherVertex = previousVertex->getDuplicate();
            if (anotherVertex != nullptr)
                dealWithAlreadyProcessedVertex(anotherVertex, newTexture, newNormal, indices, vertices);
            else
            {
                Vertex* duplicate = new Vertex(vertices.size(), previousVertex->getPosition());
                duplicate->setTextureIndex(newTexture);
                duplicate->setNormalIndex(newNormal);
                previousVertex->setDuplicate(duplicate);
                vertices.push_back(duplicate);
                indices.push_back(duplicate->getIndex());
            }
        }
    }

    void OBJFileLoader::removeUnusedVertices(std::vector<Vertex*> vertices)
    {
        for (Vertex* v : vertices)
        {
            if (!v->isSet())
            {
                v->setTextureIndex(0);
                v->setNormalIndex(0);
            }
        }
    }

}
