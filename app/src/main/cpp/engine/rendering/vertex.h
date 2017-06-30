#ifndef VERTEX_H
#define VERTEX_H

#include <engine/core/vector3f.h>
#include <engine/core/vector2f.h>

class Vertex
{
public:
	Vertex(const Vector3f &position = Vector3f(0, 0, 0), const Vector2f &textureCoordinate = Vector2f(0, 0),
		   const Vector3f &normal = Vector3f(0, 0, 0), const Vector3f &tangent = Vector3f(0, 0, 0));

	Vector3f position() const { return m_position; }
	Vector2f textureCoordinate() const { return m_textureCoordinate; }
	Vector3f normal() const { return m_normal; }
	void setNormal(const Vector3f &normal) { m_normal = normal; }
	Vector3f tangent() const { return m_tangent; }

private:
	Vector3f m_position;
	Vector2f m_textureCoordinate;
	Vector3f m_normal;
	Vector3f m_tangent;
};

#endif // VERTEX_H
