#ifndef SPHERECOLLIDER
#define SPHERECOLLIDER

#include "Collider.h"
#include "ColisionMath.h"

namespace NSPhysics {

struct SphereCollider : Collider 
{
    glm::vec3 Center;
    float radius;

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
        return FindSphereSphereCollisionPoints(
                this, selfTransform, collider, otherTransform);
    }

    CollisionPoints TestCollision(
        const Transform* selfTransform,
        const PlaneCollider* collider,
        const Transform* otherTransform) const override
    {
        return FindSpherePlaneCollisionPoints(
                this, selfTransform, collider, otherTransform);
    }
};


}

#endif
