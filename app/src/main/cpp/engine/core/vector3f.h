#ifndef VECTOR3F_H
#define VECTOR3F_H

#include "vector2f.h"
#include <string>

class Quaternion;
class Vector3f
{
public:
	Vector3f();
	Vector3f(float x, float y, float z);

	void set(float x, float y, float z) { m_x = x; m_y = y; m_z = z; }

	operator std::string() const;
	float length() const;
	float max() const;
	float dot(const Vector3f &) const;
	Vector3f abs() const;
	Vector3f cross(const Vector3f &) const;
	Vector3f normalized() const;
	Vector3f rotate(const Vector3f &, float) const;
	Vector3f rotate(const Quaternion &) const;
	Vector2f xy() const { return Vector2f(m_x, m_y); }
	Vector2f yz() const { return Vector2f(m_y, m_z); }
	Vector2f zx() const { return Vector2f(m_z, m_x); }
	Vector2f yx() const { return Vector2f(m_y, m_x); }
	Vector2f zy() const { return Vector2f(m_z, m_y); }
	Vector2f xz() const { return Vector2f(m_x, m_z); }
	Vector3f lerp(const Vector3f &, float) const;
	Vector3f operator +(const Vector3f &) const;
	Vector3f operator +(float) const;
	Vector3f operator -(const Vector3f &) const;
	Vector3f operator -(float) const;
	Vector3f operator *(const Vector3f &) const;
	Vector3f operator *(float) const;
	Vector3f operator /(const Vector3f &) const;
	Vector3f operator /(float) const;
	Vector3f &operator +=(const Vector3f &);
	bool operator ==(const Vector3f &) const;
	bool operator !=(const Vector3f &) const;

	float x() const { return m_x; }
	float y() const { return m_y; }
	float z() const { return m_z; }

protected:
	float m_x;
	float m_y;
	float m_z;
};

#endif // VECTOR3F_H
