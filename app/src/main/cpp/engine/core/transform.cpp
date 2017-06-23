#include "transform.h"

Transform::Transform() :
		m_parentTransformation(nullptr),
		m_translation(Vector3f(0, 0, 0)),
		m_rotation(Quaternion(0, 0, 0, 1)),
		m_scale(Vector3f(1, 1, 1)),
		m_hasChanged(true),
		m_parentMatrixCalculatedFirstTime(false)
{
	m_parentMatrix.initIdentity();
}

Matrix4f Transform::transformation()
{
	m_hasChanged = false;

	Matrix4f translationM;
	translationM.initTranslation(m_translation.x(), m_translation.y(), m_translation.z());

	Matrix4f rotationM;
	rotationM = m_rotation.toRotationMatrix();

	Matrix4f scaleM;
	scaleM.initScale(m_scale.x(), m_scale.y(), m_scale.z());

	return calculateParentMatrix() * translationM * rotationM * scaleM;
}

bool Transform::hasChanged() const
{
	if (m_parentTransformation != nullptr && m_parentTransformation->hasChanged())
		return true;

	return m_hasChanged;
}

void Transform::rotate(const Vector3f &axis, float angle)
{
	m_rotation = (Quaternion(axis, angle) * m_rotation).normalized();
}

Matrix4f Transform::calculateParentMatrix()
{
	if (m_parentTransformation != nullptr &&
			(!m_parentMatrixCalculatedFirstTime || m_parentTransformation->hasChanged())) {
		m_parentMatrixCalculatedFirstTime = true;
		m_parentMatrix = m_parentTransformation->transformation();
	}

	return m_parentMatrix;
}

Vector3f Transform::calculateTransformedTranslation()
{
	return calculateParentMatrix().transform(m_translation);
}

Quaternion Transform::calculateTransformedRotation()
{
	Quaternion parentRotation(0, 0, 0, 1);

	//TODO Make use of hasChanged()???
	if (m_parentTransformation != nullptr)
		parentRotation = m_parentTransformation->calculateTransformedRotation();

	return parentRotation * m_rotation;
}
