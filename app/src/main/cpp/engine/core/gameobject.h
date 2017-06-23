#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <set>
#include "transform.h"

using namespace std;

class CoreEngine;
class RenderingEngine;
class GameComponent;
class Camera;
class Shader;

class GameObject
{
public:
	GameObject();

	GameObject(const GameObject &) = delete;
	GameObject &operator =(const GameObject &) = delete;

	void onOpenGLResizedAll(int width, int height);
	void updateAll(float dt);
	void renderAll(Shader &, RenderingEngine &);

	void onOpenGLResized(int width, int height);
	void update(float dt);
	void render(Shader &, RenderingEngine &);

	void addChild(GameObject *);
	void addComponent(GameComponent *);

	void setEngine(CoreEngine *);

	Transform &transform() { return m_transform; }

	set<GameObject *> descendantsAndSelf();

private:
	CoreEngine *m_coreEngine;
	set<GameObject *> m_children;
	set<GameComponent *> m_components;
	Transform m_transform;
};

#endif // GAMEOBJECT_H
