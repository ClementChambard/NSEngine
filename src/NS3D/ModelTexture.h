#ifndef MODELTEXTURE_INCLUDED_H
#define MODELTEXTURE_INCLUDED_H

#include <GL/glew.h>

namespace NS3D {

    class ModelTexture {
        public:
            ModelTexture(GLuint texid) : textureID(texid) {}

            GLuint getID() const { return textureID; }
            float getShineDamper() const { return shineDamper; }
            float getReflectivity() const { return reflectivity; }
            bool getHasTransparency() const { return hasTransparency; }
            bool getUseFakeNormals() const { return useFakeNormals; }
            int getNumberOfRows() const { return numberOfRows; }

            ModelTexture* setShineDamper(const float sD) { shineDamper = sD; return this; }
            ModelTexture* setReflectivity(const float r) { reflectivity = r; return this; }
            ModelTexture* setSpecular(const float sD, const float r) { shineDamper = sD; reflectivity = r; return this; }
            ModelTexture* setHasTransparency(const bool b) { hasTransparency = b; return this; }
            ModelTexture* setUseFakeNormals(const bool b) { useFakeNormals = b; return this; }
            ModelTexture* setNumberOfRows(const int n) { numberOfRows = n; return this; }

        private:
            GLuint textureID;
            float shineDamper = 1;
            float reflectivity = 0;
            bool hasTransparency = false;
            bool useFakeNormals = false;
            int numberOfRows = 1;
    };

}

#endif
