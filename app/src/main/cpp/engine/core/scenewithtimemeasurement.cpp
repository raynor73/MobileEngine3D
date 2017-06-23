#include "scenewithtimemeasurement.h"

SceneWithTimeMeasurement::SceneWithTimeMeasurement() :
		m_hasPrevTimePoint(false)
{}

void SceneWithTimeMeasurement::update()
{
	auto currentTime = std::chrono::high_resolution_clock::now();

	if (m_hasPrevTimePoint) {
		std::chrono::duration<float> dt;
		dt = currentTime - m_prevTimePoint;
		m_prevTimePoint = currentTime;
		update(dt.count());
	} else {
		m_hasPrevTimePoint = true;
		m_prevTimePoint = currentTime;
	}
}
