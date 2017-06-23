#ifndef MESHRESOURCE_H
#define MESHRESOURCE_H

#include <glwrapper.h>

class MeshResource
{
public:
	MeshResource(int numberOfIndices);
	~MeshResource();

	GLuint vertexBufferObjectName() const { return m_vertexBufferObjectName; }
	GLuint indexBufferObjectName() const { return m_indexBufferObjectName; }

	int numberOfIndices() const { return m_numberOfIndices; }
	void setNumberOfIndices(int numberOfIndices) { m_numberOfIndices = numberOfIndices; }

	MeshResource(const MeshResource &) = delete;
	MeshResource &operator =(const MeshResource &) = delete;

private:
	GLuint m_vertexBufferObjectName;
	GLuint m_indexBufferObjectName;
	int m_numberOfIndices;
};

#endif // MESHRESOURCE_H
