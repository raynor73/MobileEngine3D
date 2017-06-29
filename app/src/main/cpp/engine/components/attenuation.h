#ifndef ATTENUATION_H
#define ATTENUATION_H

#include <engine/core/vector3f.h>

class Attenuation : public Vector3f
{
public:
	Attenuation(float constant, float linear, float exponent);

	float constant() const { return m_x; }
	float linear() const { return m_y; }
	float exponent() const { return m_z; }
	void setConstant(float constant) { m_x = constant; }
	void setLinear(float linear) { m_y = linear; }
	void setExponent(float exponent) { m_z = exponent; }
};

#endif // ATTENUATION_H
