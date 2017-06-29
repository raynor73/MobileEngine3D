#include "JoystickInput.h"
#include <logwrapper.h>
#include <stdexcept>
#include <sstream>

const string JoystickInput::TAG = "TestUserInput";

size_t JoystickInput::addJoystickListener(function<void(JoystickPosition)> listener)
{
	m_joystickListeners.push_back(listener);
	return m_joystickListeners.size() - 1;
}

void JoystickInput::removeJoystickListener(size_t position)
{
	if (m_joystickListeners.size() - 1 < position) {
		Log::e(TAG, "Joystick listener not found");
		throw new runtime_error("Joystick listener not found");
	}

	m_joystickListeners.erase(m_joystickListeners.begin() + position);
}

void JoystickInput::onJoystickPositionChanged(const JoystickInput::JoystickPosition &position)
{
	for (auto listener : m_joystickListeners)
		listener(position);
}
