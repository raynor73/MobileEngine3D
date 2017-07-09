#include "scenewithrootobject.h"
#include "gameobject.h"

void SceneWithRootObject::update(float dt)
{
	m_rootGameObject->updateAll(dt);
}
