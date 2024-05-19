#include "OBJFileLoader.h"
#include "../FileOpener.h"
#include "../Error.h"
#include "Vertex.h"
#include <vector>

static bool stringBeginWith(cstr s, cstr pattern)
{
    char sc = *s;
    for (char c = *pattern; c; c = *(++pattern), sc = *(++s))
        if (sc != c) return false;
    return true;
}

static std::vector<std::string> stringSplit(std::string s, const char sep = ' ')
{
    usize pos1 = 0;
    usize pos2 = 0;
    std::vector<std::string> strs;
    while (pos1 < s.size())
    {
        for (pos2 = pos1; s[pos2] != sep && pos2 < s.size(); pos2++);
        strs.push_back(s.substr(pos1, pos2-pos1));
        pos1 = pos2+1;
    }
    return strs;
}


namespace NS3D {

static void processVertex(std::vector<std::string> vertex, std::vector<Vertex *> vertices, std::vector<u32> indices);
static void dealWithAlreadyProcessedVertex(Vertex *previousVertex, i32 newTexture, i32 newNormal,
                                             std::vector<u32> indices, std::vector<Vertex *> vertices);
static void removeUnusedVertices(std::vector<Vertex*> vertices);
static u32* convertIndicesToArray(std::vector<u32> indices, u32& nb);
static f32 convertDataToArrays(std::vector<Vertex*> vertices, std::vector<glm::vec2> tex, std::vector<glm::vec3> nor, f32* vertAr, f32* texAr, f32* norAr);

ModelData* OBJFileLoader::loadOBJ(cstr fileName)
{
    if (!ns::FileOpener::openFileLines(fileName))
    {
        ns::error("Couldn't open OBJ file : ", fileName);
    }
    ns::info("[NS3D] Starting reading file ", fileName);

    std::string line;
    std::vector<Vertex*> vertices;
    std::vector<glm::vec2> texcoords;
    std::vector<glm::vec3> normals;
    std::vector<u32> indices;

    while(ns::FileOpener::readLine(line))
    {
        std::vector<std::string> currentLine = stringSplit(line);
        if (stringBeginWith(line.c_str(), "v "))
        {
            glm::vec3 vertexpos = glm::vec3(std::stof(currentLine[1]), std::stof(currentLine[2]), std::stof(currentLine[3]));
            vertices.push_back(new Vertex(vertices.size(), vertexpos));
        }
        else if (stringBeginWith(line.c_str(), "vt "))
        {
            glm::vec2 vertextex = glm::vec2(std::stof(currentLine[1]), std::stof(currentLine[2]));
            texcoords.push_back(vertextex);
        }
        else if (stringBeginWith(line.c_str(), "vn "))
        {
            glm::vec3 vertexnor = glm::vec3(std::stof(currentLine[1]), std::stof(currentLine[2]), std::stof(currentLine[3]));
            normals.push_back(vertexnor);
        }
        else if (stringBeginWith(line.c_str(), "f "))
        {
            std::vector<std::string> v1 = stringSplit(currentLine[1],'/');
            std::vector<std::string> v2 = stringSplit(currentLine[2],'/');
            std::vector<std::string> v3 = stringSplit(currentLine[3],'/');
            processVertex(v1, vertices, indices);
            processVertex(v2, vertices, indices);
            processVertex(v3, vertices, indices);
        }
    }
    ns::info("[NS3D] finished reading file.");
    removeUnusedVertices(vertices);
    f32* verticesAr = new f32[vertices.size() * 3];
    f32* texturesAr = new f32[vertices.size() * 2];
    f32* normalsAr = new f32[vertices.size() * 3];
    f32 furthestPoint = convertDataToArrays(vertices, texcoords, normals, verticesAr, texturesAr, normalsAr);
    u32 numId;
    u32* indicesAr = convertIndicesToArray(indices, numId);
    ModelData* data = new ModelData(verticesAr, texturesAr, normalsAr, indicesAr, furthestPoint, vertices.size(), indices.size());
    ns::info("[NS3D] model generated!");
    return data;
}


void processVertex(std::vector<std::string> vertex, std::vector<Vertex*> vertices, std::vector<u32> indices)
{
    u32 id = std::stoi(vertex[0]) -1;
    Vertex* currentVertex = vertices[id];
    i32 tid = std::stoi(vertex[1]) -1;
    i32 nid = std::stoi(vertex[2]) -1;
    if (!currentVertex->isSet()) {
        currentVertex->setTextureIndex(tid);
        currentVertex->setNormalIndex(nid);
    } else {
        dealWithAlreadyProcessedVertex(currentVertex, tid, nid, indices, vertices);
    }
}

u32* convertIndicesToArray(std::vector<u32> indices, u32& nb)
{
    u32* ar = new u32[indices.size()];
    for (usize i = 0; i < indices.size(); i++)
        ar[i] = indices[i];
    nb = indices.size();
    return ar;
}

f32 convertDataToArrays(std::vector<Vertex*> vertices, std::vector<glm::vec2> tex, std::vector<glm::vec3> nor, f32* vertAr, f32* texAr, f32* norAr)
{
    f32 furthestPoint = 0;
    for (usize i = 0; i < vertices.size(); i++)
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

void dealWithAlreadyProcessedVertex(Vertex* previousVertex, i32 newTexture, i32 newNormal, std::vector<u32> indices, std::vector<Vertex*> vertices)
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

void removeUnusedVertices(std::vector<Vertex*> vertices)
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
