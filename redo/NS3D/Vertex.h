#ifndef VERTEX_INCLUDED_H
#define VERTEX_INCLUDED_H

#include <glm/glm.hpp>
#include "../defines.h"

#define NO_INDEX -1

namespace NS3D {

class Vertex {
    public:
        Vertex(i32 id, const glm::vec3& pos) :
            index(id), position(pos), length(pos.length()) {}
        ~Vertex();

        i32 getIndex() const { return index; }
        f32 getLength() const { return length; }
        bool isSet() const { return textureIndex != NO_INDEX && normalIndex != NO_INDEX; }
        bool hasSameTextureAndNormal(i32 tex, i32 nor) const { return textureIndex == tex && normalIndex == nor; }

        void setTextureIndex(i32 id) { textureIndex = id; }
        void setNormalIndex(i32 id) { normalIndex = id; }
        void setDuplicate(Vertex* v) { duplicateVertex = v; }

        glm::vec3 getPosition() const { return position; }
        i32 getTextureIndex() const { return textureIndex; }
        i32 getNormalIndex() const { return normalIndex; }
        Vertex* getDuplicate() const { return duplicateVertex; }

    private:
        i32 index;
        glm::vec3 position;
        f32 length;
        i32 textureIndex = NO_INDEX;
        i32 normalIndex = NO_INDEX;
        Vertex* duplicateVertex = nullptr;
};

}

#endif
