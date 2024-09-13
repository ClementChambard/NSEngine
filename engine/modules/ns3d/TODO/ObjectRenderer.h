#ifndef OBJECTRENDERER_INCLUDED_H
#define OBJECTRENDERER_INCLUDED_H

#include "Object3D.h"
#include "StaticShader.h"
#include "OutlineShader.h"
#include <vector>
#include <map>

namespace NS3D {

class ObjectRenderer {
    public:
        ObjectRenderer(StaticShader* shad) : shader(shad), oshader(new OutlineShader()) {}
        ~ObjectRenderer() { delete shader; delete oshader; }

        void render(const std::map<const TexturedModel*, std::vector<const Object3D*>>& objects, const std::vector<const Light*>& light);
        void renderOutlined(const Object3D* obj);

    private:
        StaticShader* shader;
        OutlineShader* oshader;

        void prepareTexturedModel(const TexturedModel* model);
        void prepareInstance(const Object3D* object);
        void unbindTexturedModel();
    friend class Renderer3D;
};

}

#endif
