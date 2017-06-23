#include "gamecomponent.h"
#include <engine/core/gameobject.h>

void GameComponent::onOpenGLResized(int, int) {}

Transform &GameComponent::transform()
{
	return m_parentGameObject->transform();
}
