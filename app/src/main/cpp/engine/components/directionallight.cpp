#include "directionallight.h"
#include <engine/rendering/renderingengine.h>

DirectionalLight::DirectionalLight(RenderingEngine &renderingEngine, const Vector3f &color, float intensity,
								   shared_ptr<Shader> shader) :
		BaseLight(renderingEngine, color, intensity)
{
	m_shader = shader;
}

Vector3f DirectionalLight::direction()
{
	return transform().calculateTransformedRotation().calculateForward().normalized();
}
