#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

#include <memory>
#include <engine/core/vector3f.h>
#include <engine/components/baselight.h>
#include <engine/rendering/shader.h>

using namespace std;

class RenderingEngine;

class DirectionalLight : public BaseLight
{
public:
	DirectionalLight(RenderingEngine &renderingEngien, const Vector3f &color, float intensity,
					 shared_ptr<Shader> shader);

	Vector3f direction();
};

#endif // DIRECTIONALLIGHT_H
