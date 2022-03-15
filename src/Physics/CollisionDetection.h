#ifndef COLLISIONDETECTION
#define COLLISIONDETECTION

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace NSPhysics {
 
    struct CollisionPoints {
        glm::vec3 A;        //Furthest point of A into B
        glm::vec3 B;        //Furthest point of B into A
        glm::vec3 Normal;   // B - A normalized
        float Depth;        //Length of B - A
        bool HasCollision;
    };

    struct Transform {
        glm::vec3 Position;
        glm::vec3 Scale;
        glm::quat Rotation;
    };

}

#endif
