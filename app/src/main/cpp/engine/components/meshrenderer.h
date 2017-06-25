#ifndef MESHRENDERER_H
#define MESHRENDERER_H

#include <engine/components/gamecomponent.h>
#include <engine/rendering/mesh.h>
#include <engine/rendering/material.h>
#include <engine/rendering/shader.h>

class MeshRenderer : public GameComponent
{
public:
	MeshRenderer(Mesh *mesh, Material *material);

	void render(Shader &shader, RenderingEngine &) override;

private:
	Mesh *m_mesh;
	Material *m_material;
};

#endif // MESHRENDERER_H
