#ifndef VERTEX_H
#define VERTEX_H

#include <engine/core/vector3f.h>
#include <engine/core/vector2f.h>

class Vertex
{
public:
	static const int SIZE = 8;

	Vertex(const Vector3f &position);
	Vertex(const Vector3f &position, const Vector2f &textureCoordinate);
	Vertex(const Vector3f &position, const Vector2f &textureCoordinate, const Vector3f &normal);

	Vector3f position() const { return m_position; }
	Vector2f textureCoordinate() const { return m_textureCoordinate; }
	Vector3f normal() const { return m_normal; }
	void setNormal(const Vector3f &normal) { m_normal = normal; }

private:
	Vector3f m_position;
	Vector2f m_textureCoordinate;
	Vector3f m_normal;
};

#endif // VERTEX_H
