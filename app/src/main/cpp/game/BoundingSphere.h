#ifndef BOUNDINGSPHERE_H
#define BOUNDINGSPHERE_H


#include <engine/core/vector3f.h>

class BoundingSphere
{
public:
	BoundingSphere(const Vector3f &center, float radius) :
			m_center(center),
			m_radius(radius) {}

	Vector3f center() const { return m_center; }
	float radius() const { return m_radius; }

private:
	const Vector3f m_center;
	const float m_radius;
};


#endif //BOUNDINGSPHERE_H
