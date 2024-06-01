#ifndef TEXTUREDMODEL_INCLUDED_H
#define TEXTUREDMODEL_INCLUDED_H

#include "RawModel.h"
#include "ModelTexture.h"

namespace NS3D {

class TexturedModel {
    public:
        TexturedModel(RawModel* mod, ModelTexture* tex) : model(mod), texture(tex) {}
        ~TexturedModel() {}

        RawModel* getRawModel() const { return model; }
        ModelTexture* getModelTexture() const { return texture; }

    private: 
        RawModel* model;
        ModelTexture* texture;

};

}

#endif
