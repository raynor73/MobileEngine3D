#include "camera.h"
//#include <engine/rendering/renderingengine.h>
#include <engine/core/coreengine.h>
#include <engine/core/transform.h>

const Vector3f Camera::yAxis(0, 1, 0);

Camera::Camera(float fov, float aspectRatio, float zNear, float zFar) : GameComponent()
{
	m_projection.initPerspective(fov, aspectRatio, zNear, zFar);
}

Matrix4f Camera::calculateViewProjection()
{
	Matrix4f cameraRotation = transform().calculateTransformedRotation().conjugate().toRotationMatrix();

	Matrix4f cameraTranslation;
	Vector3f position = transform().calculateTransformedTranslation();
	cameraTranslation.initTranslation(-position.x(), -position.y(), -position.z());

	return m_projection * cameraRotation * cameraTranslation;
}

void Camera::addToEngine(CoreEngine &engine)
{
	//engine.renderingEngine().setCamera(this);
}
