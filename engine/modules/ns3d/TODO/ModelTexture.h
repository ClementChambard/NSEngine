#ifndef MODELTEXTURE_INCLUDED_H
#define MODELTEXTURE_INCLUDED_H

#include <GL/glew.h>
#include "../defines.h"

namespace NS3D {

class ModelTexture {
    public:
        ModelTexture(GLuint texid) : textureID(texid) {}

        GLuint getID() const { return textureID; }
        f32 getShineDamper() const { return shineDamper; }
        f32 getReflectivity() const { return reflectivity; }
        bool getHasTransparency() const { return hasTransparency; }
        bool getUseFakeNormals() const { return useFakeNormals; }
        i32 getNumberOfRows() const { return numberOfRows; }

        ModelTexture* setShineDamper(const f32 sD) { shineDamper = sD; return this; }
        ModelTexture* setReflectivity(const f32 r) { reflectivity = r; return this; }
        ModelTexture* setSpecular(const f32 sD, const f32 r) { shineDamper = sD; reflectivity = r; return this; }
        ModelTexture* setHasTransparency(const bool b) { hasTransparency = b; return this; }
        ModelTexture* setUseFakeNormals(const bool b) { useFakeNormals = b; return this; }
        ModelTexture* setNumberOfRows(const i32 n) { numberOfRows = n; return this; }

    private:
        GLuint textureID;
        f32 shineDamper = 1;
        f32 reflectivity = 0;
        bool hasTransparency = false;
        bool useFakeNormals = false;
        i32 numberOfRows = 1;
};

}

#endif
