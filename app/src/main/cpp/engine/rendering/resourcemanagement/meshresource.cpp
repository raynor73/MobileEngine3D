#include "meshresource.h"

MeshResource::MeshResource(int numberOfIndices) :
	m_numberOfIndices(numberOfIndices)
{
	glGenBuffers(1, &m_vertexBufferObjectName);
	glGenBuffers(1, &m_indexBufferObjectName);
}

MeshResource::~MeshResource()
{
	if (glIsBuffer(m_vertexBufferObjectName) == GL_TRUE)
		glDeleteBuffers(1, &m_vertexBufferObjectName);

	if (glIsBuffer(m_indexBufferObjectName) == GL_TRUE)
		glDeleteBuffers(1, &m_indexBufferObjectName);
}
