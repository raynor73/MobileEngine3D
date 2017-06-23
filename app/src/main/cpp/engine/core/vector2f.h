#ifndef VECTOR2F_H
#define VECTOR2F_H

#include <string>

class Vector2f
{
public:
	Vector2f();
	Vector2f(float x, float y);

	float x() const { return m_x; }
	float y() const { return m_y; }

	operator std::string() const;
	float length() const;
	float dot(const Vector2f &) const;
	float max() const;
	Vector2f lerp(const Vector2f &, float) const;
	Vector2f abs() const;
	Vector2f normalized() const;
	Vector2f rotate(float) const;
	float cross(const Vector2f &) const;
	Vector2f operator +(const Vector2f &) const;
	Vector2f operator +(float) const;
	Vector2f operator -(const Vector2f &) const;
	Vector2f operator -(float) const;
	Vector2f operator *(const Vector2f &) const;
	Vector2f operator *(float) const;
	Vector2f operator /(const Vector2f &) const;
	Vector2f operator /(float) const;
	bool operator ==(const Vector2f &) const;

private:
	float m_x;
	float m_y;
};

#endif // VECTOR2F_H
