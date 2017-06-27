#ifndef CAMERA_H
#define CAMERA_H

#include <engine/core/vector3f.h>
#include <engine/core/matrix4f.h>
#include <engine/components/gamecomponent.h>

class CoreEngine;
class Camera : public GameComponent
{
public:
	static const Vector3f yAxis;

	Camera(float fov, float aspectRatio, float zNear, float zFar);

	virtual void addToEngine(CoreEngine &) override;

	Matrix4f calculateViewProjection();
	void reset(float fov, float aspectRatio, float zNear, float zFar);

private:
	Matrix4f m_projection;
};

#endif // CAMERA_H
