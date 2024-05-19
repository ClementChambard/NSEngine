#include "ObjectList.h"
#include "NS3D.h"
#include <sstream>
#include "../TextureManager.h"
#include "../Error.h"
#include <string>

namespace NS3D {

    std::map<usize, ModelTexture*> ObjectList::modelTextures;
    std::map<cstr, RawModel*> ObjectList::rawModels;
    std::vector<TexturedModel*> ObjectList::texturedModels;

    template<typename T>
    std::string ObLi_PtrStr(T* in) { std::ostringstream s; s << &(*in); return s.str(); }

    TexturedModel* ObjectList::model(usize i) { return texturedModels[i]; }

    void ObjectList::LoadModel(cstr filename, usize texID)
    {
        if (rawModels.find(filename) == rawModels.end()) rawModels[filename] = OBJLoader::loadObjModel(filename);
        if (modelTextures.find(texID) == modelTextures.end()) modelTextures[texID] = MTex(ns::TextureManager::GetTextureID(texID));
        texturedModels.push_back(new TexturedModel(rawModels[filename], modelTextures[texID]));
        ns::info("Loaded model :", filename, "at", ObLi_PtrStr(&rawModels[filename]), "with texture", texID, "at", ObLi_PtrStr(&modelTextures[texID]), "  -->", ObLi_PtrStr(texturedModels.back()));
    }

    RawModel* ObjectList::GetRaw(cstr name)
    {
        if (rawModels.find(name) == rawModels.end()) rawModels[name] = OBJLoader::loadObjModel(name);
        return rawModels[name];
    }

    ModelTexture* ObjectList::GetTexture(usize id)
    {
        if (modelTextures.find(id) == modelTextures.end()) modelTextures[id] = MTex(ns::TextureManager::GetTextureID(id));
        return modelTextures[id];
    }

}
