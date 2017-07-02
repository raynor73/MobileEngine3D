#ifndef INDEXEDMODEL_H
#define INDEXEDMODEL_H

#include <vector>
#include <engine/core/vector3f.h>
#include <engine/core/vector2f.h>

using namespace std;

class IndexedModel
{
public:
	void calculateNormals();
	void calculateTangents();

	vector<Vector3f> &positions() { return m_positions; }
	vector<Vector2f> &textureCoordinates() { return m_textureCoordinates; }
	vector<Vector3f> &normals()  { return m_normals; }
	vector<Vector3f> &tangents()  { return m_tangents; }
	vector<unsigned int> &indices() { return m_indices; }

private:
	static const string TAG;

	vector<Vector3f> m_positions;
	vector<Vector2f> m_textureCoordinates;
	vector<Vector3f> m_normals;
	vector<Vector3f> m_tangents;
	vector<unsigned int> m_indices;
};

#endif // INDEXEDMODEL_H
