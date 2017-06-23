#ifndef USERINPUT_H
#define USERINPUT_H

#include <set>
#include <string>
#include <unordered_map>

using namespace std;

class UserInput
{
public:
	struct TouchEvent
	{
		enum Type { DOWN, MOVE, UP };

		TouchEvent(Type type, float x, float y) : m_type(type), m_x(x), m_y(y) {}

		Type type() const { return m_type; }
		void setType(Type type) { m_type = type; }

		float x() const { return m_x; }
		void setX(float x) { m_x = x; }

		float y() const { return m_y; }
		void setY(float y) { m_y = y; }

	private:
		Type m_type;
		float m_x;
		float m_y;
	};

	static int findIntConfigValue(const string &name);
	static void setConfigValue(const string &name, int value);

	void addTouchListener(void (*)(TouchEvent));
	void removeTouchListener(void (*)(TouchEvent));

protected:
	static const char *TAG;
	static unordered_map<string, int> s_intConfigValues;

	set<void (*)(TouchEvent)> m_touchEventListeners;
};

#endif //USERINPUT_H
