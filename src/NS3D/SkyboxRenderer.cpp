#include "SkyboxRenderer.h"
#include "../NSEngine.h"

namespace NS3D {
    
    const float SkyboxRenderer::SIZE = 800.f;

    float SkyboxRenderer::VERTICES[] = {        
	    -SIZE,  SIZE, -SIZE,
	    -SIZE, -SIZE, -SIZE,
	    SIZE, -SIZE, -SIZE,
	     SIZE, -SIZE, -SIZE,
	     SIZE,  SIZE, -SIZE,
	    -SIZE,  SIZE, -SIZE,

	    -SIZE, -SIZE,  SIZE,
	    -SIZE, -SIZE, -SIZE,
	    -SIZE,  SIZE, -SIZE,
	    -SIZE,  SIZE, -SIZE,
	    -SIZE,  SIZE,  SIZE,
	    -SIZE, -SIZE,  SIZE,

	     SIZE, -SIZE, -SIZE,
	     SIZE, -SIZE,  SIZE,
	     SIZE,  SIZE,  SIZE,
	     SIZE,  SIZE,  SIZE,
	     SIZE,  SIZE, -SIZE,
	     SIZE, -SIZE, -SIZE,

	    -SIZE, -SIZE,  SIZE,
	    -SIZE,  SIZE,  SIZE,
	     SIZE,  SIZE,  SIZE,
	     SIZE,  SIZE,  SIZE,
	     SIZE, -SIZE,  SIZE,
	    -SIZE, -SIZE,  SIZE,

	    -SIZE,  SIZE, -SIZE,
	     SIZE,  SIZE, -SIZE,
	     SIZE,  SIZE,  SIZE,
	     SIZE,  SIZE,  SIZE,
	    -SIZE,  SIZE,  SIZE,
	    -SIZE,  SIZE, -SIZE,

	    -SIZE, -SIZE, -SIZE,
	    -SIZE, -SIZE,  SIZE,
	     SIZE, -SIZE, -SIZE,
	     SIZE, -SIZE, -SIZE,
	    -SIZE, -SIZE,  SIZE,
	     SIZE, -SIZE,  SIZE
	};

    SkyboxRenderer::SkyboxRenderer(SkyboxShader* shad) : shader(shad)
    {
        Cube = Loader3D::loadToVAO(VERTICES, 108, 3);
    }

    void SkyboxRenderer::render()
    {
        if (texID == 0) return;
        shader->start();       
        shader->SetProjectionMatrix(NSEngine::activeCamera3D()->getProjection());
        shader->SetViewMatrix(NSEngine::activeCamera3D()->getView());
        glBindVertexArray(Cube->getVaoID());
        glEnableVertexAttribArray(0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, texID);
        glDrawArrays(GL_TRIANGLES, 0, Cube->getVertexCount());
        glDisableVertexAttribArray(0);
        glBindVertexArray(0);
        shader->stop();
    }

}
