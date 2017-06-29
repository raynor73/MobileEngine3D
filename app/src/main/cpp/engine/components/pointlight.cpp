#include <cmath>
#include <engine/components/pointlight.h>
#include <engine/rendering/renderingengine.h>

PointLight::PointLight(RenderingEngine &renderingEngine, const Vector3f &color, float intensity,
					   const Attenuation &attenuation, shared_ptr<Shader> shader) :
	BaseLight(renderingEngine, color, intensity),
	m_attenuation(attenuation)
{
	m_shader = shader;

	float a = m_attenuation.exponent();
	float b = m_attenuation.linear();
	float c = m_attenuation.constant() - COLOR_DEPTH * m_intensity * m_color.max();
	m_range = (-b + std::sqrt(b * b - 4 * a * c))/(2 * a);
}
