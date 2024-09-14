#ifndef OBJECTLIST_HEADER_INCLUDED
#define OBJECTLIST_HEADER_INCLUDED

#include <vector>
#include "TexturedModel.h"
#include <map>
#include <defines.h>

namespace ns::n3d {

class ObjectList {

    public:

        static TexturedModel* model(usize i);
        static void load_model(cstr filename, usize tex_id);

        static RawModel* get_raw(cstr name);
        static ModelTexture* get_texture(usize id);

        static void Cleanup() {
            for (auto tm : textured_models) delete tm;
            for (auto rm : raw_models     ) delete rm.second;
            raw_models.clear();
            model_textures.clear();
            textured_models.clear();
        }

    private:

        static std::map<cstr, RawModel*> raw_models;
        static std::map<usize, ModelTexture*> model_textures;
        static std::vector<TexturedModel*> textured_models;
};

}

#endif // OBJECTLIST_HEADER_INCLUDED
