#ifndef OBJECTLIST_INCLUDED_H
#define OBJECTLIST_INCLUDED_H

#include <vector>
#include "TexturedModel.h"
#include <map>
#include "../defines.h"

namespace NS3D {

class ObjectList {

    public:

        static TexturedModel* model(usize i);
        static void LoadModel(cstr filename, usize texID);

        static RawModel* GetRaw(cstr name);
        static ModelTexture* GetTexture(usize id);

        static void Cleanup() {
            for (auto tm : texturedModels) delete tm;
            for (auto rm : rawModels     ) delete rm.second;
            rawModels.clear();
            modelTextures.clear();
            texturedModels.clear();
        }

    private:

        static std::map<cstr, RawModel*> rawModels;
        static std::map<usize, ModelTexture*> modelTextures;
        static std::vector<TexturedModel*> texturedModels;
};

}

#endif // OBJECTLIST_INCLUDED_H
