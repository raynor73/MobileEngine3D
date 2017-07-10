#include "vector3f.h"
#include <cmath>
#include "quaternion.h"
#include <float.h>
#include <sstream>

Vector3f::Vector3f() :
		m_x(0),
		m_y(0),
		m_z(0)
{}

Vector3f::Vector3f(float x, float y, float z) :
		m_x(x),
		m_y(y),
		m_z(z)
{}

Vector3f Vector3f::lerp(const Vector3f &destination, float lerpFactor) const
{
	return ((destination - (*this)) * lerpFactor) + (*this);
}

float Vector3f::length() const
{
	return sqrtf(m_x * m_x + m_y * m_y + m_z * m_z);
}

float Vector3f::max() const
{
	return std::max(m_x, std::max(m_y ,m_z));
}

float Vector3f::dot(const Vector3f &other) const
{
	return m_x * other.m_x + m_y * other.m_y + m_z * other.m_z;
}

Vector3f Vector3f::abs() const
{
	return Vector3f(std::abs(m_x), std::abs(m_y), std::abs(m_z));
}

Vector3f Vector3f::cross(const Vector3f &other) const
{
	float newX = m_y * other.m_z - m_z * other.m_y;
	float newY = m_z * other.m_x - m_x * other.m_z;
	float newZ = m_x * other.m_y - m_y * other.m_x;

	return Vector3f(newX, newY, newZ);
}

Vector3f Vector3f::normalized() const
{
	return Vector3f(*this) / length();
}

Vector3f Vector3f::rotate(const Vector3f &axis, float angle) const
{
	float sinAngle = std::sin(-angle);
	float cosAngle = std::cos(-angle);

	return this->cross(axis * sinAngle) + ((*this) * cosAngle) + (axis * (this->dot(axis * (1 - cosAngle))));
}

Vector3f Vector3f::rotate(const Quaternion &rotation) const
{
	Quaternion conjugate = rotation.conjugate();

	Quaternion w = rotation * (*this) * conjugate;

	return Vector3f(w.x(), w.y(), w.z());
}

Vector3f Vector3f::operator +(const Vector3f &other) const
{
	return Vector3f(m_x + other.m_x, m_y + other.m_y, m_z + other.m_z);
}

Vector3f Vector3f::operator +(float a) const
{
	return Vector3f(m_x + a, m_y + a, m_z + a);
}

Vector3f Vector3f::operator -(const Vector3f &other) const
{
	return Vector3f(m_x - other.m_x, m_y - other.m_y, m_z - other.m_z);
}

Vector3f Vector3f::operator -(float a) const
{
	return Vector3f(m_x - a, m_y - a, m_z - a);
}

Vector3f Vector3f::operator *(const Vector3f &other) const
{
	return Vector3f(m_x * other.m_x, m_y * other.m_y, m_z * other.m_z);
}

Vector3f Vector3f::operator *(float a) const
{
	return Vector3f(m_x * a, m_y * a, m_z * a);
}

Vector3f Vector3f::operator /(const Vector3f &other) const
{
	return Vector3f(m_x / other.m_x, m_y / other.m_y, m_z / other.m_z);
}

Vector3f Vector3f::operator /(float a) const
{
	return Vector3f(m_x / a, m_y / a, m_z / a);
}

Vector3f::operator std::string() const
{
	std::ostringstream stringStream;
	stringStream << "Vector3f: " << m_x << "; " << m_y << "; " << m_z;
	return stringStream.str();
}

Vector3f &Vector3f::operator +=(const Vector3f &other)
{
	m_x += other.m_x;
	m_y += other.m_y;
	m_z += other.m_z;

	return *this;
}

bool Vector3f::operator ==(const Vector3f &other) const
{
	return
			std::abs(m_x - other.m_x) < FLT_EPSILON &&
			std::abs(m_y - other.m_y) < FLT_EPSILON &&
			std::abs(m_z - other.m_z) < FLT_EPSILON;
}

bool Vector3f::operator !=(const Vector3f &other) const
{
	return !((*this) == other);
}
