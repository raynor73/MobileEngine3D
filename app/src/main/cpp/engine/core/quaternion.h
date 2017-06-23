#ifndef QUATERNION_H
#define QUATERNION_H

#include "vector3f.h"
#include "matrix4f.h"

class Quaternion
{
public:
	Quaternion(const Matrix4f &);
	Quaternion(float x, float y, float z, float w);
	Quaternion(const Vector3f &axis, float angle);

	Matrix4f toRotationMatrix() const;

	Vector3f calculateForward() const;
	Vector3f calculateBack() const;
	Vector3f calculateUp() const;
	Vector3f calculateDown() const;
	Vector3f calculateRight() const;
	Vector3f calculateLeft() const;

	float length() const;
	Quaternion normalized() const;
	Quaternion conjugate() const;
	float dot(const Quaternion &) const;
	Quaternion nlerp(const Quaternion &destination, float factor, bool shortestRequired);
	Quaternion slerp(const Quaternion &destination, float factor, bool shortestRequired);
	Quaternion operator *(const Quaternion &) const;
	Quaternion operator -(const Quaternion &) const;
	Quaternion operator +(const Quaternion &) const;
	Quaternion operator *(const Vector3f &) const;
	Quaternion operator *(float) const;
	bool operator ==(const Quaternion &) const;
	bool operator !=(const Quaternion &) const;

	float x() const { return m_x; }
	void setX(float x) { m_x = x; }
	
	float y() const { return m_y; }
	void setY(float y) { m_y = y; }
	
	float z() const { return m_z; }
	void setZ(float z) { m_z = z; }
	
	float w() const { return m_w; }
	void setW(float w) { m_w = w; }

private:
	float m_x;
	float m_y;
	float m_z;
	float m_w;
};

#endif // QUATERNION_H
