#ifndef OBJFILELOADER_INCLUDED_H
#define OBJFILELOADER_INCLUDED_H

#include "ModelData.h"
#include "Vertex.h"
#include <string>
#include <vector>

namespace NS3D {

    class OBJFileLoader {
        public:
            static ModelData* loadOBJ(std::string fileName);

        private:
            static void processVertex(std::vector<std::string> vertex, std::vector<Vertex*> vertices, 
                    std::vector<unsigned int> indices);
            static unsigned int* convertIndicesToArray(std::vector<unsigned int> indices, unsigned int& nb);
            static float convertDataToArrays(std::vector<Vertex*> vertices, std::vector<glm::vec2> tex, 
                    std::vector<glm::vec3> normals, float* verticesAr, float* texAr, float* normalsAr);
            static void dealWithAlreadyProcessedVertex(Vertex* previousVertex, int newTexture, int newNormal, 
                    std::vector<unsigned int> indices, std::vector<Vertex*> vertices);
            static void removeUnusedVertices(std::vector<Vertex*> vertices);
    };

}

#endif
