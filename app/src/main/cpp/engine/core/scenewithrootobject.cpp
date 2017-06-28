#include "scenewithrootobject.h"
#include "gameobject.h"

void SceneWithRootObject::update(float dt)
{
	m_rootGameObject->update(dt);
}

void SceneWithRootObject::setEngine(CoreEngine *engine) {
	m_rootGameObject->setEngine(engine);
}
