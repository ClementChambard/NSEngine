#ifndef OBJECTLIST_INCLUDED_H
#define OBJECTLIST_INCLUDED_H

#include <vector>

#include "TexturedModel.h"
#include <map>
#include <string>

namespace NS3D {

    class ObjectList {

        public:

            static TexturedModel* model(size_t i);
            static void LoadModel(std::string const& filename, size_t texID);

            static RawModel* GetRaw(std::string const& name);
            static ModelTexture* GetTexture(size_t id);

            static void Cleanup() {
                for (auto tm : texturedModels) delete tm;
                for (auto rm : rawModels     ) delete rm.second;
                rawModels.clear();
                modelTextures.clear();
                texturedModels.clear();
            }

        private:

            static std::map<std::string, RawModel*> rawModels;
            static std::map<size_t, ModelTexture*> modelTextures;
            static std::vector<TexturedModel*> texturedModels;
    };

}

#endif // OBJECTLIST_INCLUDED_H
