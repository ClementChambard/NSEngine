#ifndef SKYBOXRENDERER_INCLUDED_H
#define SKYBOXRENDERER_INCLUDED_H

#include "Loader3D.h"
#include "SkyboxShader.h"

namespace NS3D {

class SkyboxRenderer {
    public:
        SkyboxRenderer(SkyboxShader* shad);
        ~SkyboxRenderer() { delete shader; delete Cube; }

        void render();

        void setTexture(GLuint tex) { texID = tex; }

    private:
        static const f32 SIZE;
        static f32 VERTICES[];

        RawModel* Cube;
        GLuint texID = 0;
	    SkyboxShader* shader;
};

}

#endif
