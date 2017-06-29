#ifndef TESTCONTROLLER_H
#define TESTCONTROLLER_H


#include "JoystickInput.h"
#include <functional>

using namespace std;

class TestController
{
public:
	TestController(JoystickInput &leftJoystickInput, JoystickInput &rightJoystickInput);
	~TestController();

	float calculateYawRotationSpeed();
	float calculatePitchRotationSpeed();
	float calculateMoveSpeed();
	float calculateStrafeSpeed();

private:
	JoystickInput &m_rightJoystickInput;
	JoystickInput &m_leftJoystickInput;
	JoystickInput::JoystickPosition m_rightJoystickPosition;
	JoystickInput::JoystickPosition m_leftJoystickPosition;
	size_t m_rightJoystickListenerPosition;
	function<void(JoystickInput::JoystickPosition)> m_rightJoystickListener =
			[this] (JoystickInput::JoystickPosition joystickPosition)
			{
				m_rightJoystickPosition = joystickPosition;
			};
	size_t m_leftJoystickListenerPosition;
	function<void(JoystickInput::JoystickPosition)> m_leftJoystickListener =
			[this] (JoystickInput::JoystickPosition joystickPosition)
			{
				m_leftJoystickPosition = joystickPosition;
			};
};


#endif //TESTCONTROLLER_H
