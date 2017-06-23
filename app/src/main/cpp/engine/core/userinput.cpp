#include "userinput.h"
#include <logwrapper.h>

const char *UserInput::TAG = "UserInput";
unordered_map<string, int> UserInput::s_intConfigValues;

int UserInput::findIntConfigValue(const string &name)
{
	return s_intConfigValues[name];
}

void UserInput::setConfigValue(const string &name, int value)
{
	s_intConfigValues[name] = value;
}

void UserInput::addTouchListener(void (*listener)(TouchEvent))
{
	if (m_touchEventListeners.find(listener) != m_touchEventListeners.end()) {
		Log::e(TAG, "Can't add touch listener twice");
		throw new runtime_error("Can't add touch listener twice");
	}

	m_touchEventListeners.insert(listener);
}

void UserInput::removeTouchListener(void (*listener)(TouchEvent))
{
	if (m_touchEventListeners.find(listener) == m_touchEventListeners.end()) {
		Log::e(TAG, "Touch listener not found");
		throw new runtime_error("Touch listener not found");
	}

	m_touchEventListeners.erase(listener);
}
