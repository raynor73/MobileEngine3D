#ifndef LIGHTSFACTORY_H
#define LIGHTSFACTORY_H

#include <engine/components/directionallight.h>
#include <engine/components/pointlight.h>
#include <engine/components/spotlight.h>

class RenderingEngine;
class Vector3f;

class LightsFactory
{
public:
	static DirectionalLight *createDirectionalLight(RenderingEngine &renderingEngine, const Vector3f &color,
													float intensity);
	static PointLight *createPointLight(RenderingEngine &renderingEngine, const Vector3f &color, float intensity,
										const Attenuation &attenuation);
	static SpotLight *createSpotLight(RenderingEngine &renderingEngine, const Vector3f &color, float intensity,
									  const Attenuation &attenuation, float cutoff);
};

#endif // LIGHTSFACTORY_H
