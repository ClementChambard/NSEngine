#ifndef PLANECOLLIDER
#define PLANECOLLIDER

#include "Collider.h"
#include "ColisionMath.h"

namespace NSPhysics {

struct PlaneCollider : Collider {
    glm::vec3 Plane;
    float distance;

    CollisionPoints TestCollision(
        const Transform* selfTransform,
        const Collider* collider,
        const Transform* otherTransform) const override
    {
        return collider->TestCollision(otherTransform, this, selfTransform);
    }

    CollisionPoints TestCollision(
        const Transform* selfTransform,
        const SphereCollider* collider,
        const Transform* otherTransform) const override
    {
        return FindPlaneSphereCollisionPoints(
            this, selfTransform, collider, otherTransform);
    }

    CollisionPoints TestCollision(
        const Transform* selfTransform,
        const PlaneCollider* collider,
        const Transform* otherTransform) const override
    {
        return {};
    }
};

}

#endif
