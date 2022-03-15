#ifndef COLLIDER
#define COLLIDER

#include "CollisionDetection.h"

namespace NSPhysics {

struct SphereCollider;
struct PlaneCollider;

struct Collider {
    virtual CollisionPoints TestCollision(
        const Transform* selfTransform,
        const Collider* collider,
        const Transform* otherTransform) const = 0;

    virtual CollisionPoints TestCollision(
        const Transform* selfTransform,
        const SphereCollider* collider,
        const Transform* otherTransform) const = 0;

    virtual CollisionPoints TestCollision(
        const Transform* selfTransform,
        const PlaneCollider* collider,
        const Transform* otherTransform) const = 0;
};

}

#endif
