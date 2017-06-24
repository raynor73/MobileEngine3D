#ifndef MAPPEDVALUES_H
#define MAPPEDVALUES_H

#include <unordered_map>
#include <string>
#include <engine/core/vector3f.h>

class MappedValues
{
public:
	void addVector3f(const std::string &name, const Vector3f &vector) { m_vectors3f[name] = vector; }
	Vector3f findVector3f(const std::string &name) const { return m_vectors3f.at(name); }

	void addFloat(const std::string &name, float value) { m_floats[name] = value; }
	float findFloat(const std::string &name) const { return m_floats.at(name); };

protected:
	std::unordered_map<std::string, Vector3f> m_vectors3f;
	std::unordered_map<std::string, float> m_floats;
};

#endif // MAPPEDVALUES_H
