#include "lightsfactory.h"
#include <engine/rendering/renderingengine.h>

shared_ptr<DirectionalLight> LightsFactory::createDirectionalLight(RenderingEngine &renderingEngine,
																   const Vector3f &color, float intensity)
{
#ifdef __ANDROID__
	auto shader = make_shared<Shader>(renderingEngine.shadersDirPath(), "forwarddirectional");
#else
	auto shader = make_shared<Shader>(renderingEngine.shadersDirPath(), "forwarddirectional",
									  renderingEngine.vertexArrayName());
#endif

	return make_shared<DirectionalLight>(renderingEngine, color, intensity, shader);
}

shared_ptr<PointLight> LightsFactory::createPointLight(RenderingEngine &renderingEngine, const Vector3f &color,
													   float intensity, const Attenuation &attenuation)
{
#ifdef __ANDROID__
	auto shader = make_shared<Shader>(renderingEngine.shadersDirPath(), "forwardpoint");
#else
	auto shader = make_shared<Shader>(renderingEngine.shadersDirPath(), "forwardpoint",
									  renderingEngine.vertexArrayName());
#endif

	return make_shared<PointLight>(renderingEngine, color, intensity, attenuation, shader);
}

shared_ptr<SpotLight> LightsFactory::createSpotLight(RenderingEngine &renderingEngine, const Vector3f &color,
													 float intensity, const Attenuation &attenuation, float cutoff)
{
#ifdef __ANDROID__
	auto shader = make_shared<Shader>(renderingEngine.shadersDirPath(), "forwardspot");
#else
	auto shader = make_shared<Shader>(renderingEngine.shadersDirPath(), "forwardspot",
									  renderingEngine.vertexArrayName());
#endif

	return make_shared<SpotLight>(renderingEngine, color, intensity, attenuation, cutoff, shader);
}
