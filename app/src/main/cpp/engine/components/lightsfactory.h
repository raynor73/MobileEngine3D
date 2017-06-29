#ifndef LIGHTSFACTORY_H
#define LIGHTSFACTORY_H

#include <memory>
#include <engine/components/directionallight.h>
/*#include <engine/components/pointlight.h>
#include <engine/components/spotlight.h>*/

using namespace std;

class RenderingEngine;
class Vector3f;

class LightsFactory
{
public:
	static shared_ptr<DirectionalLight> createDirectionalLight(RenderingEngine &renderingEngine, const Vector3f &color,
															   float intensity);
	/*static shared_ptr<PointLight> createPointLight(RenderingEngine &renderingEngien, const Vector3f &color,
												   float intensity, const Attenuation &attenuation);
	static shared_ptr<SpotLight> createSpotLight(RenderingEngine &renderingEngien, const Vector3f &color,
												 float intensity, const Attenuation &attenuation, float cutoff);*/
};

#endif // LIGHTSFACTORY_H
