#include "ObjectList.h"
#include "N3D.h"
#include "TexManager.h"
#include "OBJLoader.h"
#include <logger.h>

namespace ns::n3d {

std::map<usize, ModelTexture*> ObjectList::model_textures;
std::map<cstr, RawModel*> ObjectList::raw_models;
std::vector<TexturedModel*> ObjectList::textured_models;

TexturedModel* ObjectList::model(usize i) { return textured_models[i]; }

void ObjectList::load_model(cstr filename, usize tex_id)
{
    if (raw_models.find(filename) == raw_models.end())
        raw_models[filename] = OBJLoader::load_obj_model(filename);
    if (model_textures.find(tex_id) == model_textures.end())
        model_textures[tex_id] = m_tex(TexManager::get_gl_texture(tex_id));
    textured_models.push_back(new TexturedModel(raw_models[filename], model_textures[tex_id]));
    NS_INFO("Loaded model : %s at %p with texture %d at %p  --> %p", filename, raw_models[filename], tex_id, model_textures[tex_id], textured_models.back());
}

RawModel* ObjectList::get_raw(cstr name)
{
    if (raw_models.find(name) == raw_models.end())
        raw_models[name] = OBJLoader::load_obj_model(name);
    return raw_models[name];
}

ModelTexture* ObjectList::get_texture(usize id)
{
    if (model_textures.find(id) == model_textures.end()) model_textures[id] = m_tex(TexManager::get_gl_texture(id));
    return model_textures[id];
}

}
