#include "NSRigGraphicsLine.h"
#include "../DrawFuncs.h"

namespace NSEngine {

    NSRigGraphicsLine::NSRigGraphicsLine(glm::vec3 const& p1, glm::vec3 const& p2, float s)
    {
        pos1 = p1;
        pos2 = p2;
        size = s;
    }

    NSRigGraphicsLine::~NSRigGraphicsLine() {}

    void NSRigGraphicsLine::Draw(const glm::mat4 &mat)
    {
        glm::vec4 p1_4 = mat * glm::vec4(pos1, 1.f);
        glm::vec4 p2_4 = mat * glm::vec4(pos2, 1.f);
        glm::vec3 p1 =  glm::vec3(p1_4/p1_4.w);
        glm::vec3 p2 =  glm::vec3(p2_4/p2_4.w);
        if (p1 != p2) draw_line_3d(p1.x, p1.y, p1.z, p2.x, p2.y, p2.z, size);
    }

}
