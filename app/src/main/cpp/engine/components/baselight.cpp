#include <engine/components/baselight.h>
#include <engine/rendering/renderingengine.h>
#include <engine/core/coreengine.h>

BaseLight::BaseLight(RenderingEngine &renderingEngine, const Vector3f &color, float intensity) :
		GameComponent(),
		m_renderingEngine(renderingEngine),
		m_color(color),
		m_intensity(intensity)
{}

void BaseLight::addToEngine(CoreEngine &engine)
{
	engine.renderingEngine().addLight(this);
}
