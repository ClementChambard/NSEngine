#include "ObjectList.h"
#include "../NSEngine.h"
#include "NS3D.h"
#include <sstream>

namespace NS3D {

    std::map<size_t, ModelTexture*> ObjectList::modelTextures;
    std::map<std::string, RawModel*> ObjectList::rawModels;
    std::vector<TexturedModel*> ObjectList::texturedModels;

    template<typename T>
    std::string ObLi_PtrStr(T* in) { std::ostringstream s; s << &(*in); return s.str(); }

    TexturedModel* ObjectList::model(size_t i) { return texturedModels[i]; }

    void ObjectList::LoadModel(const std::string &filename, size_t texID)
    {
        if (rawModels.find(filename) == rawModels.end()) rawModels[filename] = OBJLoader::loadObjModel(filename);
        if (modelTextures.find(texID) == modelTextures.end()) modelTextures[texID] = MTex(NSEngine::TextureManager::GetTextureID(texID));
        texturedModels.push_back(new TexturedModel(rawModels[filename], modelTextures[texID]));
        NSEngine::info("Loaded model : " + filename + " at " + ObLi_PtrStr(&rawModels[filename]) + " with texture " + std::to_string(texID) + " at " + ObLi_PtrStr(&modelTextures[texID]) + "   --> " + ObLi_PtrStr(texturedModels.back()));
    }

    RawModel* ObjectList::GetRaw(std::string const& name)
    {
        if (rawModels.find(name) == rawModels.end()) rawModels[name] = OBJLoader::loadObjModel(name);
        return rawModels[name];
    }

    ModelTexture* ObjectList::GetTexture(size_t id)
    {
        if (modelTextures.find(id) == modelTextures.end()) modelTextures[id] = MTex(NSEngine::TextureManager::GetTextureID(id));
        return modelTextures[id];
    }

}
