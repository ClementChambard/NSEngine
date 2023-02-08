#ifndef VERTEX_INCLUDED_H
#define VERTEX_INCLUDED_H

#include <glm/glm.hpp>

#define NO_INDEX -1

namespace NS3D {

    class Vertex {
        public:
            Vertex(int id, const glm::vec3& pos) :
                index(id), position(pos), length(pos.length()) {}
            ~Vertex();

            int getIndex() const { return index; }
            float getLength() const { return length; }
            bool isSet() const { return textureIndex != NO_INDEX && normalIndex != NO_INDEX; }
            bool hasSameTextureAndNormal(int tex, int nor) const { return textureIndex == tex && normalIndex == nor; }

            void setTextureIndex(int id) { textureIndex = id; }
            void setNormalIndex(int id) { normalIndex = id; }
            void setDuplicate(Vertex* v) { duplicateVertex = v; }

            glm::vec3 getPosition() const { return position; }
            int getTextureIndex() const { return textureIndex; }
            int getNormalIndex() const { return normalIndex; }
            Vertex* getDuplicate() const { return duplicateVertex; }

        private:
            glm::vec3 position;
            int textureIndex = NO_INDEX;
            int normalIndex = NO_INDEX;
            Vertex* duplicateVertex = nullptr;
            int index;
            float length;
    };

}

#endif
