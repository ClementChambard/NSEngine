#include "World.h"

#include <algorithm>

namespace NSPhysics {

    World::World() {}
    World::~World() {}

    void World::AddObject(Object* object)
    {
        objects.push_back(object);
    }

    void World::RemoveObject(Object* object)
    {
        if (!object) return;
        auto itr = std::find(objects.begin(), objects.end(), object);
        if (itr == objects.end()) return;
        objects.erase(itr);
    }
    
    void World::AddSolver(Solver* solver)
    {
        solvers.push_back(solver);
    }

    void World::RemoveSolver(Solver* solver)
    {
        if (!solver) return;
        auto itr = std::find(solvers.begin(), solvers.end(), solver);
        if (itr == solvers.end()) return;
        solvers.erase(itr);
    }

    void World::Step(float dt)
    {
        ResolveCollisions(dt);

        for (Object* obj : objects)
        {
            Rigidbody* o = (Rigidbody*) obj;
            o->Force += o->Mass * glm::vec3(0,9.81f,0);

            o->Velocity += o->Force / o->Mass * dt;
            o->transform->Position += o->Velocity * dt;

            o->Force = glm::vec3(0,0,0);
        }
    }

    void World::ResolveCollisions(float dt)
    {
        std::vector<Collision> collisions;
        std::vector<Collision> triggers;
        for (Object* a : objects)
        {
            for (Object* b : objects) 
            {
                if (a == b) break;
                if (!a->collider || !b->collider) continue;
                CollisionPoints points = a->collider->TestCollision(a->transform, b->collider, b->transform);
                if (points.HasCollision) {
                    bool trigger = a->IsTrigger || b->IsTrigger;
                    if (trigger) triggers  .emplace_back(a, b, points);
                    else         collisions.emplace_back(a, b, points);
                }
            }
        }

        for (Solver* solver : solvers)
        {
            solver->Solve(collisions, dt);
        }
    }

}
