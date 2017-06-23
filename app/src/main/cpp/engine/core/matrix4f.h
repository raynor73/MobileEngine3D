#ifndef MATRIX4F_H
#define MATRIX4F_H

#include <array>
#include "vector3f.h"

class Matrix4f
{
public:
	static const int SIZE = 4;
	static const int DATA_SIZE = SIZE * SIZE;

	std::array<float, DATA_SIZE> getM() const;
	void setM(const std::array<float, DATA_SIZE> &);
	float get(int row, int column) const;
	void set(int row, int column, float value);
	Matrix4f &initIdentity();
	Matrix4f &initTranslation(float, float, float);
	Matrix4f &initRotation(float, float, float);
	Matrix4f &initScale(float, float, float);
	Matrix4f &initPerspective(float fov, float aspectRatio, float zNear, float zFar);
	Matrix4f &initOrthographic(float left, float right, float bottom, float top, float near, float far);
	Matrix4f &initRotation(const Vector3f &, const Vector3f &);
	Matrix4f &initRotation(const Vector3f &forward, const Vector3f &up, const Vector3f &right);
	
	Matrix4f operator *(const Matrix4f &);
	Matrix4f &operator =(const Matrix4f &);

	Vector3f transform(const Vector3f &) const;

private:
	std::array<float, DATA_SIZE> m_data;

	inline int offset(int row, int column) const;
};

#endif // MATRIX4F_H
