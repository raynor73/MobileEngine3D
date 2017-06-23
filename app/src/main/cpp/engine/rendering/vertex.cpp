#include "vertex.h"

Vertex::Vertex(const Vector3f &position) :
	m_position(position),
	m_textureCoordinate(Vector2f(0, 0)),
	m_normal(Vector3f(0, 0, 0))
{}

Vertex::Vertex(const Vector3f &position, const Vector2f &textureCoordinate) :
	m_position(position),
	m_textureCoordinate(textureCoordinate),
	m_normal(Vector3f(0, 0, 0))
{}

Vertex::Vertex(const Vector3f &position, const Vector2f &textureCoordinate, const Vector3f &normal) :
	m_position(position),
	m_textureCoordinate(textureCoordinate),
	m_normal(normal)
{}
