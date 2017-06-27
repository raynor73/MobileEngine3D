#ifndef RENDERINGENGINE_H
#define RENDERINGENGINE_H

#include <unordered_map>
#include <list>
#include <memory>
#include <engine/components/camera.h>
#include <engine/core/gameobject.h>
#include <engine/rendering/resourcemanagement/mappedvalues.h>
#include <glwrapper.h>
#include "material.h"

using namespace std;

/*class BaseLight;
class Material;*/

class RenderingEngine : public MappedValues
{
public:
	RenderingEngine(const string &shadersDirPath);
	virtual ~RenderingEngine();

	void onOpenGLResized(GameObject &gameObject, int width, int height);
	void render(GameObject &gameObject);

	Camera &mainCamera() const;
	//BaseLight &activeLight() { return *m_activeLight; }
	GLuint vertexArrayName() const { return m_vertexArrayName; }
	GLuint samplerSlot(const string &name) const { return m_samplerMap.at(name); }
	string shadersDirPath() const { return m_shadersDirPath; }

	//void addLight(BaseLight *light) { m_lights.push_back(light); }
	void setCamera(Camera *camera) { m_mainCamera = camera; }

	virtual void updateUniformStruct(Transform &transform, Material &material, Shader &shader,
									 const string &unifotmType, const string &uniformName);

private:
	static const string TAG;

	GLuint m_vertexArrayName;
	Camera *m_mainCamera;
	shared_ptr<Shader> m_forwardAmbientShader;
	unordered_map<string, GLuint> m_samplerMap;
	string m_shadersDirPath;

	/*list<BaseLight *> m_lights;
	BaseLight *m_activeLight;*/

	string openGLVersion();
	void setClearColor(const Vector3f &color);
	void enableTextures(bool isEnabled);
	void unbindTextures();
};

#endif // RENDERINGENGINE_H
