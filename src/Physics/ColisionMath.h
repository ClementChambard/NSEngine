#ifndef COLLISIONMATH
#define COLLISIONMATH

#include "Collider.h"

namespace NSPhysics {
    
    CollisionPoints FindSphereSphereCollisionPoints(
        const SphereCollider* a, const Transform* ta,
        const SphereCollider* b, const Transform* tb);

    CollisionPoints FindSpherePlaneCollisionPoints(
        const SphereCollider* a, const Transform* ta,
        const PlaneCollider* b, const Transform* tb);

    CollisionPoints FindPlaneSphereCollisionPoints(
        const PlaneCollider* a, const Transform* ta,
        const SphereCollider* b, const Transform* tb);

}

#endif
