#ifndef SCENEWITHTIMEMEASUREMENT_H
#define SCENEWITHTIMEMEASUREMENT_H

#include "scene.h"
#include <chrono>

class SceneWithTimeMeasurement : public Scene
{
public:
	SceneWithTimeMeasurement();

	virtual void update() override;
	virtual void update(float dt) = 0;

private:
	bool m_hasPrevTimePoint;
	std::chrono::high_resolution_clock::time_point m_prevTimePoint;
};

#endif // SCENEWITHTIMEMEASUREMENT_H
