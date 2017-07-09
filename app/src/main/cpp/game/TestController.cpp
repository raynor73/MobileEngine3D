#include "TestController.h"

TestController::TestController(JoystickInput &leftJoystickInput, JoystickInput &rightJoystickInput) :
		m_leftJoystickInput(leftJoystickInput),
		m_rightJoystickInput(rightJoystickInput)
{
	m_rightJoystickListenerPosition = m_rightJoystickInput.addJoystickListener(m_rightJoystickListener);
	m_leftJoystickListenerPosition = m_leftJoystickInput.addJoystickListener(m_leftJoystickListener);
}

TestController::~TestController()
{
	m_rightJoystickInput.removeJoystickListener(m_rightJoystickListenerPosition);
	m_leftJoystickInput.removeJoystickListener(m_leftJoystickListenerPosition);
}

float TestController::calculateYawRotationSpeed()
{
	return m_rightJoystickPosition.x() * 180;
}

float TestController::calculatePitchRotationSpeed()
{
	return m_rightJoystickPosition.y() * 180;
}

float TestController::calculateMoveSpeed()
{
	return -m_leftJoystickPosition.y() * 100;
}

float TestController::calculateStrafeSpeed()
{
	return m_leftJoystickPosition.x() * 100;
}
