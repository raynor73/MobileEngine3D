#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <memory>
#include "vector3f.h"
#include "matrix4f.h"
#include "quaternion.h"

using namespace std;

class Transform
{
public:
	Transform();

	Vector3f translation() const { return m_translation; }
	void setTranslation(const Vector3f &translation) { m_hasChanged = true; m_translation = translation; }

	Quaternion rotation() const { return m_rotation; }
	void setRotation(const Quaternion &rotation) { m_hasChanged = true; m_rotation = rotation; }

	Vector3f scale() const { return m_scale; }
	void setScale(const Vector3f &scale) { m_hasChanged = true; m_scale = scale; }

	Matrix4f transformation();

	Transform *parentTransformation() { return m_parentTransformation; }
	void setParentTransformation(Transform *parentTransformation) { m_parentTransformation = parentTransformation; }

	bool hasChanged() const;

	void rotate(const Vector3f &axis, float angle);

	Vector3f calculateTransformedTranslation();
	Quaternion calculateTransformedRotation();

private:
	Transform *m_parentTransformation;
	Matrix4f m_parentMatrix;

	Vector3f m_translation;
	Quaternion m_rotation;
	Vector3f m_scale;

	bool m_hasChanged;
	bool m_parentMatrixCalculatedFirstTime;

	Matrix4f calculateParentMatrix();
};

#endif // TRANSFORM_H
