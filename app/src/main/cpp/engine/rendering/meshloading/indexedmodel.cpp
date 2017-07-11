#include <logwrapper.h>
#include <utils.h>
#include <cmath>
#include <float.h>
#include "indexedmodel.h"

using namespace std;

const string IndexedModel::TAG = "IndexedModel";

void IndexedModel::calculateNormals()
{
	for (int i = 0; i < m_indices.size(); i += 3) {
		int i0 = m_indices[i];
		int i1 = m_indices[i + 1];
		int i2 = m_indices[i + 2];

		Vector3f v1 = m_positions[i1] - m_positions[i0];
		Vector3f v2 = m_positions[i2] - m_positions[i0];

		Vector3f normal = v1.cross(v2).normalized();

		m_normals[i0] += normal;
		m_normals[i1] += normal;
		m_normals[i2] += normal;
	}

	for (int i = 0; i < m_normals.size(); i++)
		m_normals[i] = m_normals[i].normalized();
}

void IndexedModel::calculateTangents()
{
	for (int i = 0; i < m_indices.size(); i += 3) {
		int i0 = m_indices[i];
		int i1 = m_indices[i + 1];
		int i2 = m_indices[i + 2];

		Vector3f edge1 = m_positions[i1] - m_positions[i0];
		Vector3f edge2 = m_positions[i2] - m_positions[i0];

		float deltaU1 = m_textureCoordinates[i1].x() - m_textureCoordinates[i0].x();
		float deltaV1 = m_textureCoordinates[i1].y() - m_textureCoordinates[i0].y();
		float deltaU2 = m_textureCoordinates[i2].x() - m_textureCoordinates[i0].x();
		float deltaV2 = m_textureCoordinates[i2].y() - m_textureCoordinates[i0].y();

		float dividend = deltaU1 * deltaV2 - deltaU2 * deltaV1;
		//TODO: The first 0.0f may need to be changed to 1.0f here.
		float f = abs(dividend) < FLT_EPSILON ? 0.0f : 1.0f / dividend;
		Vector3f tangent;
		tangent.setX(f * (deltaV2 * edge1.x() - deltaV1 * edge2.x()));
		tangent.setY(f * (deltaV2 * edge1.y() - deltaV1 * edge2.y()));
		tangent.setZ(f * (deltaV2 * edge1.z() - deltaV1 * edge2.z()));
//		Log::d(TAG, "Tangent #2: " + string(tangent));

		m_tangents[i0] += tangent;
		m_tangents[i1] += tangent;
		m_tangents[i2] += tangent;
	}

	for (int i = 0; i < m_tangents.size(); i++) {
		m_tangents[i] = m_tangents[i].normalized();
//		Log::d(TAG, "Tangent #1: " + string(m_tangents[i]));
	}
}
