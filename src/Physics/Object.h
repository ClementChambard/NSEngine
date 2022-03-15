#ifndef OBJECT
#define OBJECT

#include <glm/glm.hpp>
#include <vector>
#include <functional>

#include "Collider.h"

namespace NSPhysics {

    struct Collision;

    struct Object {
        Collider* collider;
        Transform* transform;
        bool IsDynamic;
        bool IsTrigger;

        std::function<void(Collision, float)> onCollision;
    };

    struct Rigidbody : Object
    {
        glm::vec3 Velocity;
        glm::vec3 Force;
        float Mass;

        glm::vec3 gravity;
        bool takesGravity;

        float staticFriction;
        float dynamicFriction;
        float restitutuin;
    };

    struct Collision {
        Object* ObjA;
        Object* ObjB;
        CollisionPoints Points;
    };

    class Solver {
        public:
            virtual void Solve(
                std::vector<Collision>& collisions,
                float dt) = 0;
    };

}

#endif
