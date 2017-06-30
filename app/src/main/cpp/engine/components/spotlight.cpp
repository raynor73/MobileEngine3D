#include <engine/components/spotlight.h>
#include <engine/components/attenuation.h>
#include <engine/rendering/renderingengine.h>

SpotLight::SpotLight(RenderingEngine &renderingEngine, const Vector3f &color, float intensity,
					 const Attenuation &attenuation, float cutoff, shared_ptr<Shader> shader) :
	PointLight(renderingEngine, color, intensity, attenuation, shader),
	m_cutoff(cutoff)
{}

Vector3f SpotLight:: direction()
{
	return transform().calculateTransformedRotation().calculateForward();
}
