#include "gameobject.h"
#include <engine/core/coreengine.h>
#include <engine/components/gamecomponent.h>
#include <engine/components/camera.h>

GameObject::GameObject() :
		m_coreEngine(nullptr)
{}

void GameObject::onOpenGLResizedAll(int width, int height)
{
	onOpenGLResized(width, height);

	for (auto child : m_children)
		child->onOpenGLResizedAll(width, height);
}

void GameObject::updateAll(float dt)
{
	update(dt);

	for (auto child : m_children)
		child->updateAll(dt);
}

void GameObject::renderAll(Shader &shader, RenderingEngine &renderingEngine)
{
	render(shader, renderingEngine);

	for (auto child : m_children)
		child->renderAll(shader, renderingEngine);
}

void GameObject::onOpenGLResized(int width, int height)
{
	for (auto component : m_components)
		component->onOpenGLResized(width, height);
}

void GameObject::update(float dt)
{
	for (auto component : m_components)
		component->update(dt);
}

void GameObject::render(Shader &shader, RenderingEngine &renderingEngine)
{
	for (auto component : m_components)
		component->render(/*shader, renderingEngine*/);
}

void GameObject::addChild(GameObject *child)
{
	m_children.insert(child);
	child->setEngine(m_coreEngine);
	child->transform().setParentTransformation(&m_transform);
}

void GameObject::addComponent(GameComponent *component)
{
	m_components.insert(component);
	component->setParentGameObject(this);
}

void GameObject::setEngine(CoreEngine *engine)
{
	if (m_coreEngine != engine) {
		m_coreEngine = engine;

		for (auto component : m_components)
			component->addToEngine(*engine);

		for (auto child : m_children)
			child->setEngine(engine);
	}
}

set<GameObject *> GameObject::descendantsAndSelf()
{
	set<GameObject *> result;

	for (auto child : m_children) {
		auto childResult = child->descendantsAndSelf();
		result.insert(childResult.begin(), childResult.end());
	}

	result.insert(this);

	return result;
}
