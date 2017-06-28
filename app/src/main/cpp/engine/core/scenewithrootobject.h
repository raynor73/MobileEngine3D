#ifndef SCENEWITHROOTOBJECT_H
#define SCENEWITHROOTOBJECT_H

#include <memory>
#include "scenewithtimemeasurement.h"

using namespace std;

class GameObject;
class CoreEngine;

class SceneWithRootObject : public SceneWithTimeMeasurement
{
public:
	GameObject &rootGameObject() { return *m_rootGameObject; }

	using SceneWithTimeMeasurement::update;

	void update(float dt) override;
	void setEngine(CoreEngine *) override;

protected:
	shared_ptr<GameObject> m_rootGameObject;
};

#endif // SCENEWITHROOTOBJECT_H
