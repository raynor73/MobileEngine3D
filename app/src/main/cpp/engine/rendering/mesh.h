#ifndef MESH_H
#define MESH_H

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <engine/rendering/vertex.h>
#include <engine/rendering/resourcemanagement/meshresource.h>

using namespace std;

class Mesh
{
public:
	static void clearLoadedModels() { s_loadedModels.clear(); }

	Mesh();
	Mesh(const string &path);

	void setVertices(vector<Vertex> &, const vector<unsigned int> &);
	void setVertices(vector<Vertex> &, const vector<unsigned int> &, bool);
	void draw() const;

private:
	static const string TAG;

	static unordered_map<string, weak_ptr<MeshResource>> s_loadedModels;

	shared_ptr<MeshResource> m_meshResource;

	void loadMesh(const string &path);
	void calculateNormals(vector<Vertex> &, const vector<unsigned int> &);
	void loadMeshAndPutToCache(const string &path);
};

#endif // MESH_H
