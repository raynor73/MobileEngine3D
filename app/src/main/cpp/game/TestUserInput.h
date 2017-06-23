#ifndef MOONLANDER3D_TESTUSERINPUT_H
#define MOONLANDER3D_TESTUSERINPUT_H


#include <engine/core/userinput.h>
#include <set>

using namespace std;

class TestUserInput : public UserInput
{
public:
	struct JoystickPosition
	{
		JoystickPosition() : m_x(0), m_y(0) {}
		JoystickPosition(float x, float y) : m_x(x), m_y(y) {}

		float x() const { return m_x; }
		void setX(float x) { m_x = x; }

		float y() const { return m_y; }
		void setY(float y) { m_y = y; }

	private:
		float m_x;
		float m_y;
	};

	void onJoystickPositionChanged(const JoystickPosition &);

	void addJoystickListener(void (*)(JoystickPosition));
	void removeJoystickListener(void (*)(JoystickPosition));

private:
	set<void (*)(JoystickPosition)> m_joystickListeners;
};


#endif //MOONLANDER3D_TESTUSERINPUT_H
