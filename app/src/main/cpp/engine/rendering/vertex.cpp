#include "vertex.h"

Vertex::Vertex(const Vector3f &position, const Vector2f &textureCoordinate, const Vector3f &normal,
			   const Vector3f &tangent) :
	m_position(position),
	m_textureCoordinate(textureCoordinate),
	m_normal(normal),
	m_tangent(tangent)
{}
