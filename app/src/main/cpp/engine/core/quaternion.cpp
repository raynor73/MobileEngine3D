#include <cmath>
#include <float.h>
#include "quaternion.h"

//From Ken Shoemake's "Quaternion Calculus and Fast Animation" article
Quaternion::Quaternion(const Matrix4f &rotation)
{
	float trace = rotation.get(0, 0) + rotation.get(1, 1) + rotation.get(2, 2);

	if (trace > 0) {
		float s = 0.5f / std::sqrt(trace + 1);
		m_w = 0.25f / s;
		m_x = (rotation.get(1, 2) - rotation.get(2, 1)) * s;
		m_y = (rotation.get(2, 0) - rotation.get(0, 2)) * s;
		m_z = (rotation.get(0, 1) - rotation.get(1, 0)) * s;
	} else {
		if (rotation.get(0, 0) > rotation.get(1, 1) && rotation.get(0, 0) > rotation.get(2, 2)) {
			float s = 2 * std::sqrt(1 + rotation.get(0, 0) - rotation.get(1, 1) - rotation.get(2, 2));
			m_w = (rotation.get(1, 2) - rotation.get(2, 1)) / s;
			m_x = 0.25f * s;
			m_y = (rotation.get(1, 0) + rotation.get(0, 1)) / s;
			m_z = (rotation.get(2, 0) + rotation.get(0, 2)) / s;
		} else if(rotation.get(1, 1) > rotation.get(2, 2)) {
			float s = 2 * std::sqrt(1 + rotation.get(1, 1) - rotation.get(0, 0) - rotation.get(2, 2));
			m_w = (rotation.get(2, 0) - rotation.get(0, 2)) / s;
			m_x = (rotation.get(1, 0) + rotation.get(0, 1)) / s;
			m_y = 0.25f * s;
			m_z = (rotation.get(2, 1) + rotation.get(1, 2)) / s;
		} else {
			float s = 2 * std::sqrt(1 + rotation.get(2, 2) - rotation.get(0, 0) - rotation.get(1, 1));
			m_w = (rotation.get(0, 1) - rotation.get(1, 0) ) / s;
			m_x = (rotation.get(2, 0) + rotation.get(0, 2) ) / s;
			m_y = (rotation.get(1, 2) + rotation.get(2, 1) ) / s;
			m_z = 0.25f * s;
		}
	}

	float length = std::sqrt(m_x * m_x + m_y * m_y + m_z * m_z + m_w * m_w);
	m_x /= length;
	m_y /= length;
	m_z /= length;
	m_w /= length;
}

Quaternion::Quaternion(float x, float y, float z, float w) :
	m_x(x),
	m_y(y),
	m_z(z),
	m_w(w)
{}

Quaternion::Quaternion(const Vector3f &axis, float angle)
{
	float sinHalfAngle = std::sin(angle / 2);
	float cosHalfAngle = std::cos(angle / 2);

	m_x = axis.x() * sinHalfAngle;
	m_y = axis.y() * sinHalfAngle;
	m_z = axis.z() * sinHalfAngle;
	m_w = cosHalfAngle;
}

float Quaternion::length() const
{
	return sqrtf(m_x * m_x + m_y * m_y + m_z * m_z + m_w * m_w);
}

Quaternion Quaternion::normalized() const
{
	float l = length();
	return Quaternion(m_x / l, m_y / l, m_z / l, m_w / l);
}

Quaternion Quaternion::conjugate() const
{
	return Quaternion(-m_x, -m_y, -m_z, m_w);
}

float Quaternion::dot(const Quaternion &other) const
{
	return m_x * other.m_x + m_y * other.m_y + m_z * other.m_z + m_w * other.m_w;
}

Quaternion Quaternion::nlerp(const Quaternion &destination, float factor, bool shortestRequired)
{
	Quaternion result = destination;

	if (shortestRequired && this->dot(destination) < 0)
		result = Quaternion(-destination.x(), -destination.y(), -destination.z(), -destination.w());

	return ((result - (*this)) * factor + (*this)).normalized();
}

//TODO Fix slerp
Quaternion Quaternion::slerp(const Quaternion &destination, float factor, bool shortestRequired)
{
	float cos = this->dot(destination);
	Quaternion correctedDestination = destination;

	if (shortestRequired && cos < 0) {
		cos = -cos;
		correctedDestination = Quaternion(-destination.x(), -destination.y(), -destination.z(), -destination.w());
	}

	if (std::abs(cos) >= 1 - FLT_EPSILON)
		return nlerp(correctedDestination, factor, false);

	float sin = std::sqrt(1 - cos * cos);
	float angle = std::atan2(sin, cos);
	float invertedSin =  1 / sin;

	float sourceFactor = std::sin((1 - factor) * angle) * invertedSin;
	float destinationFactor = std::sin(factor * angle) * invertedSin;

	return (((*this) * sourceFactor) + correctedDestination) * destinationFactor;
}

Quaternion Quaternion::operator *(const Quaternion &other) const
{
	float newW = m_w * other.m_w - m_x * other.m_x - m_y * other.m_y - m_z * other.m_z;
	float newX = m_x * other.m_w + m_w * other.m_x + m_y * other.m_z - m_z * other.m_y;
	float newY = m_y * other.m_w + m_w * other.m_y + m_z * other.m_x - m_x * other.m_z;
	float newZ = m_z * other.m_w + m_w * other.m_z + m_x * other.m_y - m_y * other.m_x;

	return Quaternion(newX, newY, newZ, newW);
}

Quaternion Quaternion ::operator *(const Vector3f &v) const
{
	float newW = -m_x * v.x() - m_y * v.y() - m_z * v.z();
	float newX =  m_w * v.x() + m_y * v.z() - m_z * v.y();
	float newY =  m_w * v.y() + m_z * v.x() - m_x * v.z();
	float newZ =  m_w * v.z() + m_x * v.y() - m_y * v.x();

	return Quaternion(newX, newY, newZ, newW);
}

Quaternion Quaternion::operator -(const Quaternion &other) const
{
	return Quaternion(m_x - other.m_x, m_y - other.m_y, m_z - other.m_z, m_w - other.m_w);
}

Quaternion Quaternion::operator +(const Quaternion &other) const
{
	return Quaternion(m_x + other.m_x, m_y + other.m_y, m_z + other.m_z, m_w + other.m_w);
}

Quaternion Quaternion::operator *(float multiplier) const
{
	return Quaternion(m_x * multiplier, m_y * multiplier, m_z * multiplier, m_w * multiplier);
}

Vector3f Quaternion::calculateForward() const
{
	return Vector3f(0, 0, 1).rotate(*this);
}

Vector3f Quaternion::calculateBack() const
{
	return Vector3f(0, 0, -1).rotate(*this);
}

Vector3f Quaternion::calculateUp() const
{
	return Vector3f(0, 1, 0).rotate(*this);
}

Vector3f Quaternion::calculateDown() const
{
	return Vector3f(0, -1, 0).rotate(*this);
}

Vector3f Quaternion::calculateRight() const
{
	return Vector3f(1, 0, 0).rotate(*this);
}

Vector3f Quaternion::calculateLeft() const
{
	return Vector3f(-1, 0, 0).rotate(*this);
}

Matrix4f Quaternion::toRotationMatrix() const
{
	Vector3f forward(2 * (m_x * m_z - m_w * m_y), 2 * (m_y * m_z + m_w * m_x), 1 - 2 * (m_x * m_x + m_y * m_y));
	Vector3f up(2 * (m_x * m_y + m_w * m_z), 1 - 2 * (m_x * m_x + m_z * m_z), 2.0f * (m_y * m_z - m_w * m_x));
	Vector3f right(1 - 2 * (m_y * m_y + m_z * m_z), 2 * (m_x * m_y - m_w * m_z), 2 * (m_x * m_z + m_w * m_y));

	return Matrix4f().initRotation(forward, up, right);
}

bool Quaternion::operator ==(const Quaternion &other) const
{
	return
			std::abs(m_x - other.m_x) < FLT_EPSILON &&
			std::abs(m_y - other.m_y) < FLT_EPSILON &&
			std::abs(m_z - other.m_z) < FLT_EPSILON &&
			std::abs(m_w - other.m_w) < FLT_EPSILON;
}

bool Quaternion::operator !=(const Quaternion &other) const
{
	return !((*this) == other);
}
