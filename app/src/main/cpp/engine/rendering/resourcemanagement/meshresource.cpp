#include "meshresource.h"

MeshResource::MeshResource(int numberOfIndices) :
	m_numberOfIndices(numberOfIndices)
{
	glGenBuffers(1, &m_vertexBufferObjectName);
	glGenBuffers(1, &m_indexBufferObjectName);
}

MeshResource::~MeshResource()
{
	glDeleteBuffers(1, &m_vertexBufferObjectName);
	glDeleteBuffers(1, &m_indexBufferObjectName);
}
