#ifndef GAMECOMPONENT_H
#define GAMECOMPONENT_H

class CoreEngine;
class RenderingEngine;
class Camera;
class Shader;
class Transform;
class GameObject;

class GameComponent
{
public:
	virtual void onOpenGLResized(int width, int height);
	virtual void update(float dt) {}
	virtual void render(/*Shader &, */RenderingEngine &) {}
	virtual void addToEngine(CoreEngine &) {}
	virtual void removeFromEngine(CoreEngine &) {}

	GameObject *parentGameObject() { return m_parentGameObject; }
	void setParentGameObject(GameObject *gameObject) { m_parentGameObject = gameObject; }

	Transform &transform();

protected:
	GameObject *m_parentGameObject;
};

#endif // GAMECOMPONENT_H
