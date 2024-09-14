#ifndef OBJECTRENDERER_HEADER_INCLUDED
#define OBJECTRENDERER_HEADER_INCLUDED

#include "Object3D.h"
#include "StaticShader.h"
#include "OutlineShader.h"
#include <vector>
#include <map>

namespace ns::n3d {

class ObjectRenderer {
    public:
        ObjectRenderer(StaticShader* shad) : shader(shad), oshader(new OutlineShader()) {}
        ~ObjectRenderer() { delete shader; delete oshader; }

        void render(const std::map<const TexturedModel*, std::vector<const Object3D*>>& objects, const std::vector<const Light*>& light);
        void render_outlined(const Object3D* obj);

    private:
        StaticShader* shader;
        OutlineShader* oshader;

        void prepare_textured_model(const TexturedModel* model);
        void prepare_instance(const Object3D* object);
        void unbind_textured_model();

    friend class Renderer3D;
};

}

#endif // OBJECTRENDERER_HEADER_INCLUDED
