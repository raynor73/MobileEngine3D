#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include <memory>
#include <engine/components/baselight.h>
#include <engine/components/attenuation.h>
#include <engine/core/vector3f.h>
#include <engine/rendering/shader.h>

using namespace std;

class RenderingEngine;

class PointLight : public BaseLight
{
public:
	PointLight(RenderingEngine &renderingEngine, const Vector3f &color, float intensity, const Attenuation &attenuation,
			   shared_ptr<Shader> shader);

	Attenuation attenuation() const { return m_attenuation; }
	void setAttenuation(const Attenuation &attenuation) { m_attenuation = attenuation; }
	float range() { return m_range; }

protected:
	Attenuation m_attenuation;
	float m_range;

private:
	static const int COLOR_DEPTH = 256;
};

#endif // POINTLIGHT_H
