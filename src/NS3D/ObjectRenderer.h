#ifndef OBJECTRENDERER_INCLUDED_H
#define OBJECTRENDERER_INCLUDED_H

#include "Object3D.h"
#include "StaticShader.h"
#include <vector>
#include <map>

namespace NS3D {

    class ObjectRenderer {
        public:
            ObjectRenderer(StaticShader* shad) : shader(shad) {}
            ~ObjectRenderer() { delete shader; }

            void render(const std::map<const TexturedModel*, std::vector<const Object3D*>>& objects, const std::vector<const Light*>& light);

        private:
            StaticShader* shader;

            void prepareTexturedModel(const TexturedModel* model);
            void prepareInstance(const Object3D* object);
            void unbindTexturedModel();
        friend class Renderer3D;
    };

}

#endif
