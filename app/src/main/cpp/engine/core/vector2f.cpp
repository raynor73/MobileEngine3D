#include "vector2f.h"
#include <cmath>
#include <float.h>
#include <sstream>
#include <utils.h>

Vector2f::Vector2f() :
		m_x(0),
		m_y(0)
{}

Vector2f::Vector2f(float x, float y) :
		m_x(x),
		m_y(y)
{}

float Vector2f::length() const
{
	return sqrtf(m_x * m_x + m_y * m_y);
}

float Vector2f::max() const
{
	return std::max(m_x, m_y);
}

float Vector2f::dot(const Vector2f &other) const
{
	return m_x * other.m_x + m_y * other.m_y;
}

Vector2f Vector2f::lerp(const Vector2f &destination, float lerpFactor) const
{
	return ((destination - (*this)) * lerpFactor) + (*this);
}

Vector2f Vector2f::abs() const
{
	return Vector2f(std::abs(m_x), std::abs(m_y));
}

Vector2f Vector2f::normalized() const
{
	return Vector2f(*this) / length();
}

float Vector2f::cross(const Vector2f &other) const
{
	return m_x * other.m_y - m_y * other.m_x;
}

Vector2f Vector2f::rotate(float angle) const
{
	float rad = Utils::toRadians(angle);
	float cos = cosf(rad);
	float sin = sinf(rad);
	return Vector2f(m_x * cos - m_y * sin, m_x * sin + m_y * cos);
}

Vector2f Vector2f::operator +(const Vector2f &other) const
{
	return Vector2f(m_x + other.m_x, m_y + other.m_y);
}

Vector2f Vector2f::operator +(float a) const
{
	return Vector2f(m_x + a, m_y + a);
}

Vector2f Vector2f::operator -(const Vector2f &other) const
{
	return Vector2f(m_x - other.m_x, m_y - other.m_y);
}

Vector2f Vector2f::operator -(float a) const
{
	return Vector2f(m_x - a, m_y - a);
}

Vector2f Vector2f::operator *(const Vector2f &other) const
{
	return Vector2f(m_x * other.m_x, m_y * other.m_y);
}

Vector2f Vector2f::operator *(float a) const
{
	return Vector2f(m_x * a, m_y * a);
}

Vector2f Vector2f::operator /(const Vector2f &other) const
{
	return Vector2f(m_x / other.m_x, m_y / other.m_y);
}

Vector2f Vector2f::operator /(float a) const
{
	return Vector2f(m_x / a, m_y / a);
}

Vector2f::operator std::string() const
{
	std::ostringstream stringStream;
	stringStream << "Vector2f: " << m_x << "; " << m_y << std::endl;
	return stringStream.str();
}

bool Vector2f::operator ==(const Vector2f &other) const
{
	return
			std::abs(m_x - other.m_x) < FLT_EPSILON &&
			std::abs(m_y - other.m_y) < FLT_EPSILON;
}
