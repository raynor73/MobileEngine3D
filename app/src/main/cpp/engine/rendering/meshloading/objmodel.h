#ifndef OBJMODEL_H
#define OBJMODEL_H

#include <string>
#include <vector>
#include <engine/rendering/meshloading/objindex.h>
#include <engine/rendering/meshloading/indexedmodel.h>
#include <engine/core/vector3f.h>

using namespace std;

class OBJModel
{
public:
	OBJModel(const string &path);

	IndexedModel toIndexedModel() const;

private:
	static const string TAG;

	vector<Vector3f> m_positions;
	vector<Vector2f> m_textureCoordinates;
	vector<Vector3f> m_normals;
	vector<OBJIndex> m_indices;
	bool m_hasTextureCoordinates;
	bool m_hasNormals;

	OBJIndex parseOBJIndex(const string &);
};

#endif // OBJMODEL_H
