#include <engine/rendering/meshloading/objmodel.h>
#include <logwrapper.h>
#include "mesh.h"

unordered_map<string, weak_ptr<MeshResource>> Mesh::s_loadedModels;

Mesh::Mesh() {}

Mesh::Mesh(const string &path)
{
	if (s_loadedModels.count(path) > 0) {
		weak_ptr<MeshResource> meshResource = s_loadedModels[path];
		if (meshResource.expired()) {
			s_loadedModels.erase(path);
			loadMeshAndPutToCache(path);
		} else {
			Log::i("Mesh", "Mesh with path:" + path + "already loaded, reusing buffers");
			m_meshResource = meshResource.lock();
		}
	} else {
		loadMeshAndPutToCache(path);
	}
}

void Mesh::loadMeshAndPutToCache(const string &path)
{
	loadMesh(path);
	s_loadedModels[path] = m_meshResource;
}

void Mesh::setVertices(vector<Vertex> &vertices, const vector<unsigned int> &indices)
{
	setVertices(vertices, indices, false);
}

void Mesh::setVertices(vector<Vertex> &vertices, const vector<unsigned int> &indices, bool shouldCalculateNormals)
{
	if (shouldCalculateNormals)
		calculateNormals(vertices, indices);

	m_meshResource = make_shared<MeshResource>(indices.size());

	GLsizeiptr numberOfVertices = (GLsizeiptr) vertices.size();

	auto sizeOfVertexBuffer = numberOfVertices * Vertex::SIZE * sizeof(float);
	unsigned char *temporaryVertexBuffer = new unsigned char[sizeOfVertexBuffer];
	auto m_floatBuffer = reinterpret_cast<float *>(temporaryVertexBuffer);
	for (int i = 0; i < numberOfVertices; i++) {
		int base = i * Vertex::SIZE;
		const Vector3f position = vertices[i].position();
		const Vector2f textureCoordinate = vertices[i].textureCoordinate();
		const Vector3f normal = vertices[i].normal();

		m_floatBuffer[base] = position.x();
		m_floatBuffer[base + 1] = position.y();
		m_floatBuffer[base + 2] = position.z();
		m_floatBuffer[base + 3] = textureCoordinate.x();
		m_floatBuffer[base + 4] = textureCoordinate.y();
		m_floatBuffer[base + 5] = normal.x();
		m_floatBuffer[base + 6] = normal.y();
		m_floatBuffer[base + 7] = normal.z();
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_meshResource->vertexBufferObjectName());
	glBufferData(GL_ARRAY_BUFFER, sizeOfVertexBuffer, temporaryVertexBuffer, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_meshResource->indexBufferObjectName());
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_meshResource->numberOfIndices() * sizeof(unsigned int), indices.data(),
				   GL_STATIC_DRAW);
}

void Mesh::draw()
{
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, m_meshResource->vertexBufferObjectName());
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, Vertex::SIZE * sizeof(float), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, Vertex::SIZE * sizeof(float),
							reinterpret_cast<const void *>(3 * sizeof(float)));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, Vertex::SIZE * sizeof(float),
							reinterpret_cast<const void *>(5 * sizeof(float)));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_meshResource->indexBufferObjectName());

	glDrawElements(GL_TRIANGLES, m_meshResource->numberOfIndices(), GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

void Mesh::calculateNormals(vector<Vertex> &vertices, const vector<unsigned int> &indices)
{
	for (int i = 0; i < indices.size(); i += 3) {
		int i0 = indices[i];
		int i1 = indices[i + 1];
		int i2 = indices[i + 2];

		Vector3f v1 = vertices[i1].position() - vertices[i0].position();
		Vector3f v2 = vertices[i2].position() - vertices[i0].position();

		Vector3f normal = v1.cross(v2).normalized();

		vertices[i0].normal() += normal;
		vertices[i1].normal() += normal;
		vertices[i2].normal() += normal;
	}

	for (int i = 0; i < vertices.size(); i++)
		vertices[i].setNormal(vertices[i].normal().normalized());
}

void Mesh::loadMesh(const string &path)
{
	OBJModel objModel(path);
	IndexedModel indexedModel = objModel.toIndexedModel();

	vector<Vertex> vertices;
	vector<unsigned int> indices;

	for (int i = 0; i < indexedModel.positions().size(); i++)
		vertices.push_back(Vertex(indexedModel.positions()[i], indexedModel.textureCoordinates()[i],
						   indexedModel.normals()[i]));

	for (int i = 0; i < indexedModel.indices().size(); i++)
		indices.push_back(indexedModel.indices()[i]);

	setVertices(vertices, indices, false);
}
