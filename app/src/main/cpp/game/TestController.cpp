#include "TestController.h"

TestController::TestController(JoystickInput &joystickInput) : m_joystickInput(joystickInput)
{
	m_listenerPosition = m_joystickInput.addJoystickListener(m_listener);
}

TestController::~TestController()
{
	m_joystickInput.removeJoystickListener(m_listenerPosition);
}

float TestController::calculateYawRotationSpeed()
{
	return m_joystickPosition.x() * 45;
}

float TestController::calculatePitchRotationSpeed()
{
	return m_joystickPosition.y() * 45;
}
