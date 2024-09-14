#ifndef TEXTUREDMODEL_HEADER_INCLUDED
#define TEXTUREDMODEL_HEADER_INCLUDED

#include "RawModel.h"
#include "ModelTexture.h"

namespace ns::n3d {

class TexturedModel {
    public:
        TexturedModel(RawModel* mod, ModelTexture* tex) : model(mod), texture(tex) {}
        ~TexturedModel() {}

        RawModel* get_raw_model() const { return model; }
        ModelTexture* get_model_texture() const { return texture; }

    private: 
        RawModel* model;
        ModelTexture* texture;

};

}

#endif // TEXTUREDMODEL_HEADER_INCLUDED
