#include "meshrenderer.h"
#include <engine/rendering/renderingengine.h>

MeshRenderer::MeshRenderer(Mesh *mesh, Material *material) :
	m_mesh(mesh),
	m_material(material)
{}

extern GLint g_ambientUniformLocation;
extern GLint g_mvpUniformLocation;
extern GLuint g_programReference;
void bindShader()
{
	glUseProgram(g_programReference);
}
void MeshRenderer::render(/*Shader &shader, */RenderingEngine &renderingEngine)
{
	/*shader.bind();
	shader.updateUniforms(transform(), *m_material, renderingEngine);*/

	bindShader();
	glUniformMatrix4fv(g_mvpUniformLocation, 1, GL_TRUE, renderingEngine.mainCamera().calculateViewProjection().getM().data());
	glUniform3f(g_ambientUniformLocation, 0, 0.5, 0);

	m_mesh->draw();
}
