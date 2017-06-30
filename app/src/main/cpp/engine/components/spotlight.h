#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include <memory>
#include <engine/components/pointlight.h>
#include <engine/core/vector3f.h>
#include <engine/rendering/shader.h>

using namespace std;

class RenderingEngine;
class Attenuation;

class SpotLight : public PointLight
{
public:
	SpotLight(RenderingEngine &renderingEngine, const Vector3f &color, float intensity, const Attenuation &attenuation,
			  float cutoff, shared_ptr<Shader> shader);

	Vector3f direction();
	float cutoff() const { return m_cutoff; }
	void setCutoff(float cutoff) { m_cutoff = cutoff; }

protected:
	float m_cutoff;
};

#endif // SPOTLIGHT_H
