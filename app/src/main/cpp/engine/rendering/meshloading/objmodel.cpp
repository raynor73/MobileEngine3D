#include "objmodel.h"
#include <fstream>
#include <sstream>
#include <unordered_map>

using namespace std;

OBJModel::OBJModel(const string &path) :
	m_hasTextureCoordinates(false),
	m_hasNormals(false)
{
	ifstream inputStream(path);
	string line;

	while (getline(inputStream, line)) {
		vector<string> tokens;
		stringstream ss;
		ss.str(line);
		string item;
		while (getline(ss, item, ' '))
			tokens.push_back(item);

		if (tokens[0] == "v") {
			float x = (float) atof(tokens[1].c_str());
			float y = (float) atof(tokens[2].c_str());
			float z = (float) atof(tokens[3].c_str());
			m_positions.push_back(Vector3f(x, y, z));
		} else if (tokens[0] == "vt") {
			float x = (float) atof(tokens[1].c_str());
			float y = (float) atof(tokens[2].c_str());
			m_textureCoordinates.push_back(Vector2f(x, y));
		} else if (tokens[0] == "vn") {
			float x = (float) atof(tokens[1].c_str());
			float y = (float) atof(tokens[2].c_str());
			float z = (float) atof(tokens[3].c_str());
			m_normals.push_back(Vector3f(x, y, z));
		} else if (tokens[0] == "f") {
			for (int i = 0; i < tokens.size() - 3; i++) {
				m_indices.push_back(parseOBJIndex(tokens[1]));
				m_indices.push_back(parseOBJIndex(tokens[2 + i]));
				m_indices.push_back(parseOBJIndex(tokens[3 + i]));
			}
		}
	}

	inputStream.close();
}

IndexedModel OBJModel::toIndexedModel() const
{
	IndexedModel indexedModel;
	IndexedModel normalModel;
	std::unordered_map<OBJIndex, unsigned int> resultIndexMap;
	std::unordered_map<int, unsigned int> normalIndexMap;
	std::unordered_map<int, int> indexMap;

	for (int i = 0; i < m_indices.size(); i++) {
		OBJIndex currentIndex = m_indices[i];

		Vector3f currentPosition = m_positions[currentIndex.vertexIndex()];
		Vector2f currentTextureCoordinate;
		Vector3f currentNormal;

		if (m_hasTextureCoordinates)
			currentTextureCoordinate = m_textureCoordinates[currentIndex.textureCoordinateIndex()];

		if (m_hasNormals)
			currentNormal = m_normals[currentIndex.normalIndex()];

		unsigned int modelVertexIndex;
		if (resultIndexMap.count(currentIndex) != 0) {
			modelVertexIndex = resultIndexMap[currentIndex];
		} else {
			modelVertexIndex = (int) indexedModel.positions().size();
			resultIndexMap[currentIndex] = modelVertexIndex;

			indexedModel.positions().push_back(currentPosition);
			indexedModel.textureCoordinates().push_back(currentTextureCoordinate);
			indexedModel.normals().push_back(currentNormal);
		}

		unsigned int normalModelIndex;
		if (normalIndexMap.count(currentIndex.vertexIndex()) != 0) {
			normalModelIndex = normalIndexMap[currentIndex.vertexIndex()];
		} else {
			normalModelIndex = (int) normalModel.positions().size();
			normalIndexMap[currentIndex.vertexIndex()] = normalModelIndex;

			normalModel.positions().push_back(currentPosition);
			normalModel.textureCoordinates().push_back(currentTextureCoordinate);
			normalModel.normals().push_back(currentNormal);
		}

		indexedModel.indices().push_back(modelVertexIndex);
		normalModel.indices().push_back(normalModelIndex);
		indexMap[modelVertexIndex] = normalModelIndex;
	}

	if (!m_hasNormals) {
		normalModel.calculateNormals();

		for (int i = 0; i < indexedModel.normals().size(); i++)
			indexedModel.normals()[i] = normalModel.normals()[indexMap[i]];
	}

	return indexedModel;
}

OBJIndex OBJModel::parseOBJIndex(const string &token)
{
	OBJIndex objIndex;

	vector<string> values;
	stringstream ss;
	ss.str(token);
	string item;
	while (getline(ss, item, '/'))
		values.push_back(item);

	objIndex.setVertexIndex(atoi(values[0].c_str()) - 1);

	if (values.size() > 1) {
		m_hasTextureCoordinates = true;
		objIndex.setTextureCoordinateIndex(atoi(values[1].c_str()) - 1);

		if (values.size() > 2) {
			m_hasNormals = true;
			objIndex.setNormalIndex(atoi(values[2].c_str()) - 1);
		}
	}

	return objIndex;
}
