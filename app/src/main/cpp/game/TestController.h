#ifndef TESTCONTROLLER_H
#define TESTCONTROLLER_H


#include "JoystickInput.h"
#include <functional>

using namespace std;

class TestController
{
public:
	TestController(JoystickInput &);
	~TestController();

	float calculateYawRotationSpeed();
	float calculatePitchRotationSpeed();

private:
	JoystickInput &m_joystickInput;
	JoystickInput::JoystickPosition m_joystickPosition;
	size_t m_listenerPosition;
	function<void(JoystickInput::JoystickPosition)> m_listener =
			[this] (JoystickInput::JoystickPosition joystickPosition)
			{
				m_joystickPosition = joystickPosition;
			};
};


#endif //TESTCONTROLLER_H
