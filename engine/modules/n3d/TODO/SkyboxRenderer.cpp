#include "SkyboxRenderer.h"
#include "Loader3D.h"
#include <GL/glew.h>

namespace ns::n3d {
    
const f32 SkyboxRenderer::SIZE = 800.f;

f32 SkyboxRenderer::VERTICES[] = {        
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
    cube = Loader3D::load_to_vao(VERTICES, 108, 3);
}

void SkyboxRenderer::render()
{
    if (tex_id == 0) return;
    shader->use();
    // shader->set_projection_matrix(ns::activeCamera3D()->getProjection());
    // shader->set_view_matrix(ns::activeCamera3D()->getView());
    glBindVertexArray(cube->get_vao_id());
    glEnableVertexAttribArray(0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, tex_id);
    glDrawArrays(GL_TRIANGLES, 0, cube->get_vertex_count());
    glDisableVertexAttribArray(0);
    glBindVertexArray(0);
}

}
