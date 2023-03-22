#ifndef OBJLOADER_INCLUDED_H
#define OBJLOADER_INCLUDED_H

#include "RawModel.h"
#include <glm/glm.hpp>
#include <vector>
#include <string>

namespace NS3D {

    class OBJLoader {
        public:
            static RawModel* loadObjModel(std::string fileName);

        private:
            static void processVertex(std::vector<std::string> vertex, std::vector<unsigned int>& indices, std::vector<glm::vec2>& texcoords, 
                    std::vector<glm::vec3>& normals, float* texcoordsAr, float* normalsAr);

    };

}

#endif
