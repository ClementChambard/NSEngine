#ifndef WORLD
#define WORLD

#include <vector>
#include "Object.h"

namespace NSPhysics {

    class World {
        public:
            World();
            ~World();

            void AddObject(Object* object);
            void RemoveObject(Object* object);

            void AddSolver(Solver* solver);
            void RemoveSolver(Solver* solver);

            void Step(float dt);
            void ResolveCollisions(float dt);

        private:
            std::vector<Object*> objects;
            std::vector<Solver*> solvers;
    };

}

#endif
