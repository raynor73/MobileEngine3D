#ifndef BASELIGHT_H
#define BASELIGHT_H

#include <memory>
#include <engine/core/vector3f.h>
#include <engine/components/gamecomponent.h>
#include <engine/rendering/shader.h>

using namespace std;

class RenderingEngine;
class CoreEngine;

class BaseLight : public GameComponent
{
public:
	BaseLight(RenderingEngine &renderingEngine, const Vector3f &color, float intensity);

	Vector3f color() const { return m_color; }
	float intensity() const { return m_intensity; }
	void setColor(const Vector3f &color) { m_color = color; }
	void setIntensity(float intensity) { m_intensity = intensity; }
	shared_ptr<Shader> shader() { return m_shader; }

	virtual void addToEngine(CoreEngine &) override;

protected:
	RenderingEngine &m_renderingEngine;
	Vector3f m_color;
	float m_intensity;
	shared_ptr<Shader> m_shader;
};

#endif // BASELIGHT_H
