#include "meshrenderer.h"
//#include <engine/rendering/renderingengine.h>

MeshRenderer::MeshRenderer(Mesh *mesh/*, Material *material*/) :
	m_mesh(mesh)/*,
	m_material(material)*/
{}

void MeshRenderer::render(/*Shader &shader, RenderingEngine &renderingEngine*/)
{
	/*shader.bind();
	shader.updateUniforms(transform(), *m_material, renderingEngine);
	m_mesh->draw();*/
}
