#include "TestUserInput.h"
#include <logwrapper.h>
#include <stdexcept>
#include <sstream>

const string TestUserInput::TAG = "TestUserInput";

void TestUserInput::addJoystickListener(void (*listener)(JoystickPosition))
{
	if (m_joystickListeners.find(listener) != m_joystickListeners.end()) {
		Log::e(TAG, "Can't add joystick listener twice");
		throw new runtime_error("Can't add joystick listener twice");
	}

	m_joystickListeners.insert(listener);

}

void TestUserInput::removeJoystickListener(void (*listener)(JoystickPosition))
{
	if (m_joystickListeners.find(listener) == m_joystickListeners.end()) {
		Log::e(TAG, "Joystick listener not found");
		throw new runtime_error("Joystick listener not found");
	}

	m_joystickListeners.erase(listener);
}

void TestUserInput::onJoystickPositionChanged(const TestUserInput::JoystickPosition &position)
{
	for (auto listener : m_joystickListeners)
		listener(position);
}
