#ifndef MOONLANDER3D_TESTUSERINPUT_H
#define MOONLANDER3D_TESTUSERINPUT_H


#include <vector>
#include <string>
#include <functional>

using namespace std;

class JoystickInput
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

	size_t addJoystickListener(function<void(JoystickPosition)>);
	void removeJoystickListener(size_t position);

private:
	static const string TAG;

	vector<function<void(JoystickPosition)>> m_joystickListeners;
};


#endif //MOONLANDER3D_TESTUSERINPUT_H
