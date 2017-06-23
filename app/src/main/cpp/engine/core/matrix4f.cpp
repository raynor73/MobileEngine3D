#include <cmath>
#include <utils.h>
#include "matrix4f.h"

int Matrix4f::offset(int row, int column) const
{
	return row * SIZE + column;
}

std::array<float, Matrix4f::DATA_SIZE> Matrix4f::getM() const
{
	return m_data;
}

void Matrix4f::setM(const std::array<float, DATA_SIZE> &data)
{
	m_data = data;
}

float  Matrix4f::get(int row, int column) const
{
	return m_data[offset(row, column)];
}

void Matrix4f::set(int row, int column, float value)
{
	m_data[offset(row, column)] = value;
}

Matrix4f &Matrix4f::initIdentity()
{
	m_data.fill(0);
	for (int i = 0; i < SIZE; i++) {
		m_data[offset(i, i)] = 1;
	}

	return *this;
}

Vector3f Matrix4f::transform(const Vector3f &vector) const
{
	return Vector3f(m_data[offset(0, 0)] * vector.x() + m_data[offset(0, 1)] * vector.y() + m_data[offset(0, 2)] * vector.z() + m_data[offset(0, 3)],
			m_data[offset(1, 0)] * vector.x() + m_data[offset(1, 1)] * vector.y() + m_data[offset(1, 2)] * vector.z() + m_data[offset(1, 3)],
			m_data[offset(2, 0)] * vector.x() + m_data[offset(2, 1)] * vector.y() + m_data[offset(2, 2)] * vector.z() + m_data[offset(2, 3)]);
}

Matrix4f &Matrix4f::initTranslation(float x, float y, float z)
{
	initIdentity();

	m_data[offset(0, 3)] = x;
	m_data[offset(1, 3)] = y;
	m_data[offset(2, 3)] = z;

	return *this;
}

Matrix4f &Matrix4f::initRotation(float x, float y, float z)
{
	initIdentity();

	Matrix4f rotationX, rotationY, rotationZ;

	float xRad = Utils::toRadians(x);
	float yRad = Utils::toRadians(y);
	float zRad = Utils::toRadians(z);

	rotationZ.initIdentity();
	rotationZ.set(0, 0, std::cos(zRad));
	rotationZ.set(0, 1, -std::sin(zRad));
	rotationZ.set(1, 0, std::sin(zRad));
	rotationZ.set(1, 1, std::cos(zRad));

	rotationX.initIdentity();
	rotationX.set(1, 1, std::cos(xRad));
	rotationX.set(1, 2, -std::sin(xRad));
	rotationX.set(2, 1, std::sin(xRad));
	rotationX.set(2, 2, std::cos(xRad));

	rotationY.initIdentity();
	rotationY.set(0, 0, std::cos(yRad));
	rotationY.set(0, 2, -std::sin(yRad));
	rotationY.set(2, 0, std::sin(yRad));
	rotationY.set(2, 2, std::cos(yRad));

	setM((rotationZ * rotationY * rotationX).getM());

	return *this;
}

Matrix4f &Matrix4f::initScale(float x, float y, float z)
{
	initIdentity();

	m_data[offset(0, 0)] = x;
	m_data[offset(1, 1)] = y;
	m_data[offset(2, 2)] = z;

	return *this;
}

Matrix4f &Matrix4f::initPerspective(float fov, float aspectRatio, float zNear, float zFar)
{
	initIdentity();

	auto tanHalfFov = std::tan(fov / 2);
	auto zRange = zNear - zFar;

	m_data[offset(0, 0)] = (float) (1.0 / (tanHalfFov * aspectRatio));
	m_data[offset(1, 1)] = (float) (1.0 / tanHalfFov);
	m_data[offset(3, 3)] = 0;
	m_data[offset(3, 2)] = 1;
	m_data[offset(2, 2)] = (-zNear - zFar) / zRange;
	m_data[offset(2, 3)] = 2 * zFar * zNear / zRange;

	return *this;
}

Matrix4f &Matrix4f::initOrthographic(float left, float right, float bottom, float top, float near, float far)
{
	float width = right - left;
	float height = top - bottom;
	float depth = far - near;

	m_data.fill(0);

	m_data[offset(0, 0)] = 2 / width;
	m_data[offset(1, 1)] = 2 / height;
	m_data[offset(2, 2)] = -2 / depth;

	m_data[offset(0, 3)] = -(right + left) / width;
	m_data[offset(1, 3)] = -(top + bottom) / height;
	m_data[offset(2, 3)] = -(far + near) / depth;
	m_data[offset(3, 3)] = 1;

	return *this;
}

Matrix4f &Matrix4f::initRotation(const Vector3f &forward, const Vector3f &up)
{
	Vector3f f = forward;
	f.normalized();

	Vector3f r = up;
	r.normalized();
	r = r.cross(f);

	Vector3f u = f.cross(r);

	return initRotation(f, u, r);
}

Matrix4f &Matrix4f::initRotation(const Vector3f &forward, const Vector3f &up, const Vector3f &right)
{
	initIdentity();

	Vector3f f = forward;
	//f.normalized();

	Vector3f r = right;
	//r.normalized();

	Vector3f u = up;
	//u.normalized();

	m_data[offset(0, 0)] = r.x();
	m_data[offset(0, 1)] = r.y();
	m_data[offset(0, 2)] = r.z();

	m_data[offset(1, 0)] = u.x();
	m_data[offset(1, 1)] = u.y();
	m_data[offset(1, 2)] = u.z();

	m_data[offset(2, 0)] = f.x();
	m_data[offset(2, 1)] = f.y();
	m_data[offset(2, 2)] = f.z();

	return *this;
}

Matrix4f Matrix4f::operator *(const Matrix4f &other)
{
	Matrix4f m;

	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			m.set(i, j, m_data[offset(i, 0)] * other.m_data[offset(0, j)] +
					m_data[offset(i, 1)] * other.m_data[offset(1, j)] +
					m_data[offset(i, 2)] * other.m_data[offset(2, j)] +
					m_data[offset(i, 3)] * other.m_data[offset(3, j)]);
		}
	}

	return m;
}

Matrix4f &Matrix4f::operator =(const Matrix4f &other)
{
	if (this == &other)
		return *this;

	m_data = other.m_data;

	return *this;
}
